#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "./sys_consts.h"
#include "./opcodes.h"
#include "./machine.h"
#include "./vm_mem.h"

struct ThreadInfo {
  uint16_t *instrs;
  uint64_t *operands;
  uint64_t pc;
};
typedef struct ThreadInfo ThreadInfo;


#define start_thread(thrd_info_ptr) // \
   pthread_t *thread; \
   pthread_create(thread, NULL, run_thread, thrd_info_ptr);

#define split_operands() op1 = (opers >> 32) & 0xffff; op2 = (opers >> 16) & 0xffff; op3 = opers & 0xffff; \
   printf("%" PRIx16 "\n", op1); \
   printf("%" PRIx16 "\n", op2); \
   printf("%" PRIx16 "\n", op3);
void run_thread(void *thread_info) {
  uint16_t *instrs   = ((ThreadInfo *)thread_info)->instrs;
  uint64_t *operands = ((ThreadInfo *)thread_info)->operands;
  uint64_t pc       = ((ThreadInfo *)thread_info)->pc;
  bool cond_flag    = 0; /* set by condition-checking instructions */

  uint16_t instr, op_code, op1, op2, op3;
  uint64_t opers;

  unsigned char *sp = malloc(1024*1024*50); //50Mb per-thread stack
  bool running=true;
  while(running) {
    op_code = instrs[pc];
    opers = operands[pc];
    pc++;
    //uint64_t instr = instrs[pc++];
    //uint16_t op_code = (instr >> 56) & 0xFF;
    //printf("0x%" PRIx64 "\n", instr);
    //printf("0x%" PRIx16 "\n", op_code);
    printf("Op Code: 0x%" PRIx16 "\n", op_code);
    printf("Operands: 0x%" PRIx64 "\n", opers);

    unsigned char reg_num; // used by OP_PSH, OP_POP
    switch(op_code) {
      case OP_HLT: /* Halt this thread */
        running=false;
        break;
      case OP_NOP: /* no-op */
         break;
      case OP_LW:  /* load word */
         break;
      case OP_SW: /* store word */
        break;
      case OP_JMP: /* un-conditional jump */
        pc = (unsigned char) opers;
        break;
      case OP_JPC: /* conditional jump */
        if(cond_flag) {
          reg_num = (unsigned char) opers;
          pc = regs[reg_num];
        }
        break;
      case OP_EQ: /* check if register values are equal */
        split_operands();
        if(regs[op1] == regs[op2]) cond_flag = true;
        else cond_flag = false;
        break;
      case OP_NE: /* check if registers values are not equal */
        split_operands();
        if(regs[op1] != regs[op2]) cond_flag = true;
        else cond_flag = false;
        break;
      case OP_GT: /* check if reg1 > reg2 */
        split_operands();
        if(regs[op1] > regs[op2]) cond_flag = true;
        else cond_flag = false;
        break;
      case OP_ADD: /* add */
        split_operands();
        printf("Old val: 0x%02X\t", regs[op1]);
        regs[op1] = regs[op2] + regs[op3]; // throw away last of the 2 bytes to fit value in the byte-sized register
        printf("New val: 0x%02X\n", regs[op1]);
        break;
      case OP_SUB: /* subtract */
        split_operands();
        regs[op1] = regs[op2] - regs[op3];
        break;
      case OP_MUL: /* multiply */
        split_operands();
        regs[op1] = regs[op2] * regs[op3];
        break;
      case OP_DIV: /* divide */
        split_operands();
        regs[op1] = regs[op2] / regs[op3];
        break;
      case OP_PSH: /* push onto stack */
        reg_num = (unsigned char) opers;
        *sp = regs[reg_num]; // let the compiler decide how to efficiently pull out the LSB
        sp++;
        break;
      case OP_POP: /* pop off stack */
        sp--;
        reg_num = (unsigned char) opers;
        regs[reg_num] = *sp;
        printf("Value popped: 0x%02X\n", regs[reg_num]);
        break;
      case OP_ALC: /* allocate memory */
        split_operands();
        VM_MALLOC(op1, regs[op2] << 8 | regs[op3]); //places vm pointer in regs[0] and regs[1]
        break;
      case OP_DLC: /* de-allocate memory */
        split_operands();
        printf("%d %d \n", regs[op1], regs[op2]);
        VM_FREE(regs[op1] << 8 | regs[op2]);
        break;
      case OP_PRNT: /* print char as ascii */
        split_operands();
        break;
      case OP_ADDI: /* add immediate */
        split_operands();
        regs[op1] += op2;
        break;
      case OP_SUBI: /* subtract immediate */
        split_operands();
        regs[op1] -= op2;
        break;
      case OP_MULI: /* multiply immediate */
        split_operands();
        regs[op1] *= op2;
        break;
      case OP_DIVI:  /* divide immediate */
        regs[op1] /= op2;
        break;
      case OP_LWI: /* load word immediate */
         split_operands();
         printf("Old val: 0x%02X\t Immediate value: 0x%02X\t", regs[op1], op2);
           regs[op1] = op2; // throw away the first byte to fit value in the byte-sized register
         printf("New val: 0x%02X\n", regs[op1]);
         break;
      case OP_SWI: /* store word immediate */
         break;
      case OP_THRD: /* create new thread */
        {
          ThreadInfo new_thread_info = { .instrs = instrs, .operands = operands, .pc = pc };
          start_thread(&new_thread_info);
        }
        break;
      case OP_CPR:
        split_operands();
        regs[op2] = regs[op1];
        break;
      default:
         break;
    }
  }

  //pthread_exit(0);
}

#define combine_ops(op1, op2, op3) ((uint64_t)op1 << 32) | ((uint64_t)op2 << 16) | ((uint64_t) op3)
#define create_instr(op_code, op1, op2, op3) \
  instrs = realloc(instrs, sizeof(uint16_t)*(i+1)); \
  operands = realloc(operands, sizeof(uint64_t)*(i+1)); \
  instrs[i] = op_code; \
  operands[i] = combine_ops(op1, op2, op3); \
  i++ \

void main(void) {
  size_t num_regs = 1024;
  setup_machine(num_regs);

  #define NUM_INSTRS
  int i = 0;
  //uint16_t instrs[NUM_INSTRS];
  //uint64_t operands[NUM_INSTRS];
  uint16_t *instrs = malloc(sizeof(uint16_t));
  uint64_t *operands = malloc(sizeof(uint64_t));

  create_instr(OP_LWI, 0x01, 1, 0);
  create_instr(OP_LWI, 0x02, 2, 0);
  create_instr(OP_ADD, 0x03, 0x02, 0x01);
  create_instr(OP_PSH, 0, 0, 0x03);
  create_instr(OP_POP, 0, 0, 0x04);
  create_instr(OP_ALC, 0x0, 0x10, 0);
  create_instr(OP_CP, 0x01, 0x03, 0);
  create_instr(OP_CP, 0x02, 0x04, 0);
  create_instr(OP_ALC, 0x0, 0x123, 0);
  create_instr(OP_DLC, 0x0, 0x02, 0);
  create_instr(OP_DLC, 0x0, 0x04, 0);
  create_instr(OP_HLT, 0, 0, 0);

  ThreadInfo thread_info = { .instrs = instrs, .operands = operands, .pc = 0 };
  run_thread(&thread_info);
}

