#include <stdio.h>
#include <stdlib.h>

#include "core/log.h"
#include "vm/instr.h"
#include "vm/io.h"
#include "vm/memory.h"
#include "vm/vm.h"

void vm_init(VM *vm) {
  signal(SIGINT, vm_io_handle_interrupt);
  vm_io_disable_input_buffering();

  /* 131072 byte | 128 kb */
  for (int i = 0; i < VM_MEMORY_MAX; ++i) {
    vm->memory[i] = 0;
  }

  for (int i = 0; i < R_COUNT; ++i) {
    vm->reg[i] = 0;
  }

  vm->reg[R_COND] = FL_ZRO;
  vm->reg[R_PC] = VM_PC_START;
  vm->running = false;
}

void vm_run(VM *vm) {
  vm->running = true;

  while (vm->running) {
    /* FETCH - Fetch Stage */
    uint16_t pc = vm->reg[R_PC]++;
    uint16_t instr = vm_mem_read(vm, pc);
    uint16_t op = instr >> 12;

    switch (op) {
    case OP_ADD:
      vm_op_add(vm, instr);
      break;
    case OP_AND:
      vm_op_and(vm, instr);
      break;
    case OP_NOT:
      vm_op_not(vm, instr);
      break;
    case OP_BR:
      vm_op_br(vm, instr);
      break;
    case OP_JMP:
      vm_op_jmp(vm, instr);
      break;
    case OP_JSR:
      vm_op_jsr(vm, instr);
      break;
    case OP_LD:
      vm_op_ld(vm, instr);
      break;
    case OP_LDI:
      vm_op_ldi(vm, instr);
      break;
    case OP_LDR:
      vm_op_ldr(vm, instr);
      break;
    case OP_LEA:
      vm_op_lea(vm, instr);
      break;
    case OP_ST:
      vm_op_st(vm, instr);
      break;
    case OP_STI:
      vm_op_sti(vm, instr);
      break;
    case OP_STR:
      vm_op_str(vm, instr);
      break;
    case OP_TRAP:
      switch (instr & 0xFF) {
      case TRAP_GETC:
        vm_op_trap_getc(vm);
        break;
      case TRAP_OUT:
        vm_op_trap_out(vm);
        break;
      case TRAP_PUTS:
        vm_op_trap_puts(vm);
        break;
      case TRAP_IN:
        vm_op_trap_in(vm);
        break;
      case TRAP_PUTSP:
        vm_op_trap_putsp(vm);
        break;
      case TRAP_HALT:
        vm_op_trap_halt(vm);
        break;
      default:
        log_message(LOG_LEVEL_ERROR,
                    "Error: unknown TRAP vector 0x%02X at PC=0x%04X\n",
                    instr & 0xFF, pc);
        vm->running = false;
        break;
      }
      break;
    case OP_RES:
      vm_op_res(vm);
      break;
    case OP_RTI:
      vm_op_rti(vm);
      break;
    default:
      log_message(LOG_LEVEL_ERROR,
                  "Error: unknown instruction in vector 0x%02X at PC=0x%04X\n",
                  instr & 0xFF, pc);
      vm->running = false;
      break;
    }
  }
  vm_io_restore_input_buffering();
}
