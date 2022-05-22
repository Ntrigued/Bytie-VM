
#ifndef OPCODES_H
#define OPCODES_H

// based on https://github.com/kkestell/tiny-vm
#define OP_HLT     0x01 /* Halt */
#define OP_NOP     0x02 /* no-op */
#define OP_LW      0x03 /* load word */
#define OP_SW      0x04 /* store word */
#define OP_JMP     0x05 /* un-conditional jump */
#define OP_JPC     0x06 /* conditional jump */
#define OP_EQ      0x07 /* check if register values are equal */
#define OP_NE      0x08 /* check if register values are not equal */
#define OP_GT      0x09 /* check if reg1 > reg2 */
#define OP_ADD     0x0A /* add */
#define OP_SUB     0x0B /* subtract */
#define OP_MUL     0x0C /* multiply */
#define OP_DIV     0x0D /* divide */
#define OP_PSH     0x0E /* push onto stack */
#define OP_POP     0x0F /* pop off stack */
#define OP_ALC     0x10 /* allocate memory */
#define OP_DLC     0x11 /* de-allocate memory */
#define OP_PRNT    0x12 /* print char as ascii */
#define OP_ADDI    0x13 /* add immediate */
#define OP_SUBI    0x14 /* subtract immediate */
#define OP_MULI    0x15 /* multiply immediate */
#define OP_DIVI    0x16 /* divide immediate */
#define OP_LWI     0x17 /* load immediate */
#define OP_SWI     0x18 /* store immediate */
#define OP_THRD    0x19 /* create thread */
#define OP_CPR     0x20 /* copy from reg1 to reg2 */

#define OP_LP      0x21  /* load pointer */
#define OP_SP      0x22  /* store pointer */
#define OP_EQP     0x23 /* check if ptr_reg values are equal */
#define OP_NEP     0x24 /* check if ptr_reg values are not equal */
#define OP_GTP     0x25 /* For ptr_regs, check if reg1 > reg2 */
#define OP_ADP     0x26 /* add (on pointer register) */
#define OP_SBP     0x27 /* subtract (on pointer register) */
#define OP_MLP     0x28 /* multiply (on pointer register) */
#define OP_DVP     0x29 /* divide (on pointer register) */
#define OP_ADPI    0x2A /* add immediate (on pointer register) */
#define OP_SBPI    0x2B /* subtract immediate (on pointer register) */
#define OP_MLPI    0x2C /* multiply immediate (on pointer register) */
#define OP_DVPI    0x2D /* divide immediate (on pointer register) */
#define OP_CPP     0x2E /* copy from reg1 to reg2 (on pointer registers) */

#endif
