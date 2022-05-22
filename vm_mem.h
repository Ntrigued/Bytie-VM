#ifndef VM_MEM_H
#define VM_MEM_H


#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./machine.h"

#define NULL_PTR (void *)0
#define MAX_UINT16 65535

void *all_mem;

struct MemListNode {
  uint16_t offset;  // VM can alloc up to 64Kb of memory
  size_t size;
  struct MemListNode *prev, *next;
  bool is_free;
};
typedef struct MemListNode MemListNode;

struct MemData {
  void *mem_region;
  MemListNode *mem_list;
};
typedef struct MemData MemData;

MemData *mem_data;
void setup_mem(void);

void vm_malloc(size_t ptr_reg_num, size_t requested_size);
void vm_free(size_t vm_ptr);

#define COMBINE_ADJ_NODES(n1, n2) \
  n1->size = n1->size + n2->size; \
  n1->next = n2->next; \
  free(n2);

#define VM_MALLOC(ptr_reg_num, requested_size) vm_malloc(ptr_reg_num, requested_size);
#define VM_FREE(ptr_reg_num) vm_free(ptr_reg_num)

#endif
