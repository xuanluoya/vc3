
#include <stdio.h>
#include <stdlib.h>

#include "vm/io.h"
#include "vm/memory.h"

void vm_mem_write(VM *vm, uint16_t address, uint16_t val) {
  vm->memory[address] = val;
}

uint16_t vm_mem_read(VM *vm, uint16_t address) {
  if (address == MMR_KBSR) {
    if (vm_io_check_key()) {
      vm->memory[MMR_KBSR] = (1 << 15);
      vm->memory[MMR_KBDR] = getchar();
    } else {
      vm->memory[MMR_KBSR] = 0;
    }
  }
  return vm->memory[address];
}
