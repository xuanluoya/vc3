#ifndef VC3_VM_MEMORY_H
#define VC3_VM_MEMORY_H

#include <stdint.h>

#include "vm/vm.h"

uint16_t vm_mem_read(VM *vm, uint16_t address);
void vm_mem_write(VM *vm, uint16_t address, uint16_t val);

#endif
