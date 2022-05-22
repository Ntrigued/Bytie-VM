#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include "./sys_consts.h"
#include "./machine.h"
#include "./vm_mem.h"

void setup_machine(size_t num_registers, size_t num_reg_ptrs) {
  regs = calloc(num_registers, sizeof(unsigned char));
  void **ptr_regs = calloc(num_reg_ptrs, sizeof(void *));
  setup_mem();
}
