#ifndef MACHINE_H
#define MACHINE_H

#include <inttypes.h>
#include <stddef.h>

#define WORD_SIZE 8 // VM will have 8-byte word size

unsigned char *regs;
void *ptr_regs; // Register for pointer data type

void setup_machine(size_t num_registers, size_t num_ptr_regs);

#endif
