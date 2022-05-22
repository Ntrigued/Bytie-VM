#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "./machine.h"
#include "./sys_consts.h"
#include "./vm_mem.h"
#ifdef DEBUG
  #include <signal.h>
#endif
#include <signal.h>

void setup_mem(void) {
  mem_data = malloc(sizeof(MemData));
  mem_data->mem_region = malloc(MAX_UINT16);

  mem_data->mem_list = malloc(sizeof(MemListNode));
  mem_data->mem_list->offset = 0;
  mem_data->mem_list->size = MAX_UINT16;
  mem_data->mem_list->prev = NULL_PTR;
  mem_data->mem_list->next = NULL_PTR;
  mem_data->mem_list->is_free = true;
}

void vm_malloc(size_t, ptr_reg_num, size_t requested_size) {
  MemListNode *mem_node = mem_data->mem_list;
  while(mem_node != NULL_PTR) {
    printf("Boop\n");
    if(mem_node->is_free && mem_node->size >= requested_size) {
      printf("Allooc VM Pointer: %" PRIx16 "\n", mem_node->offset);
      ptr_regs[ptr_reg_num] = mem_node->mem_region + mem_node->offset;
      MemListNode *new_node = malloc(sizeof(MemListNode));
      new_node->offset = mem_node->offset;
      new_node->size = mem_node->size - requested_size;
      mem_node->offset += requested_size;
      mem_node->size -= requested_size;
      new_node->prev = mem_node->prev;
      new_node->next = mem_node;
      new_node->is_free = false;

      if(mem_node->prev != NULL_PTR) new_node->prev->next = new_node;
      mem_node->prev = new_node;
      mem_node->size = requested_size;
      if(mem_node == mem_data->mem_list) {
        printf("Whacka\n");
        mem_data->mem_list = mem_node->prev;
      }
      //raise(SIGINT);
      return;
    }
    mem_node = mem_node->next;
  }

  //TODO Error handling
 }

void vm_free(size_t ptr_reg_num, uint16_t vm_ptr) {
  printf("Free VM Pointer: %" PRIx16 "\n", vm_ptr);
  MemListNode *mem_node = mem_data->mem_list;
  size_t offset = ptr_regs[ptr_reg_num] - mem_node->mem_region;
  while(mem_node->offset != offset) mem_node = mem_node->next;

  mem_node->is_free = true;
  if(mem_node->prev != NULL_PTR && mem_node->prev->is_free) { COMBINE_ADJ_NODES(mem_node->prev, mem_node); }
  if(mem_node->next != NULL_PTR && mem_node->next->is_free) { COMBINE_ADJ_NODES(mem_node, mem_node->next); }
}
