#include <stdio.h>
#include <stdlib.h>

#include "core/log.h"
#include "core/util.h"
#include "vm/instr.h"
#include "vm/memory.h"

/*
 * LC3 doc  : https://www.jmeiners.com/lc3-vm/supplies/lc3-isa.pdf
 * r0       : destination register (DR)
 * r1       : first operand (SR1)
 * imm_flag : whether we are in immediate mode
 */

static void vm_update_flags(VM *vm, uint16_t reg) {
  if (vm->reg[reg] == 0) {
    vm->reg[R_COND] = FL_ZRO;
  } else if (vm->reg[reg] >> 15) {
    vm->reg[R_COND] = FL_NEG;
  } else {
    vm->reg[R_COND] = FL_POS;
  }
}

void vm_op_add(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
    vm->reg[r0] = vm->reg[r1] + imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    vm->reg[r0] = vm->reg[r1] + vm->reg[r2];
  }
  vm_update_flags(vm, r0);
}

void vm_op_and(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
    vm->reg[r0] = vm->reg[r1] & imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    vm->reg[r0] = vm->reg[r1] & vm->reg[r2];
  }
  vm_update_flags(vm, r0);
}

void vm_op_not(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  vm->reg[r0] = ~vm->reg[r1];
  vm_update_flags(vm, r0);
}

void vm_op_br(VM *vm, uint16_t instr) {
  uint16_t pc_offset = sign_extend((instr) & 0x1ff, 9);
  uint16_t cond_flag = (instr >> 9) & 0x7;
  if (cond_flag & vm->reg[R_COND]) {
    vm->reg[R_PC] += pc_offset;
  }
}

void vm_op_jmp(VM *vm, uint16_t instr) {
  /* Also handles RET */
  uint16_t r1 = (instr >> 6) & 0x7;
  vm->reg[R_PC] = vm->reg[r1];
}

void vm_op_jsr(VM *vm, uint16_t instr) {
  uint16_t long_flag = (instr >> 11) & 1;
  vm->reg[R_R7] = vm->reg[R_PC];
  if (long_flag) {
    uint16_t long_pc_offset = sign_extend(instr & 0x7FF, 11);
    vm->reg[R_PC] += long_pc_offset; /* JSR */
  } else {
    uint16_t r1 = (instr >> 6) & 0x7;
    vm->reg[R_PC] = vm->reg[r1]; /* JSRR */
  }
}

void vm_op_ld(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  vm->reg[r0] = vm_mem_read(vm, vm->reg[R_PC] + pc_offset);
  vm_update_flags(vm, r0);
}

void vm_op_ldi(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  uint16_t addr = vm_mem_read(vm, vm->reg[R_PC] + pc_offset);
  vm->reg[r0] = vm_mem_read(vm, addr);

  vm_update_flags(vm, r0);
}

void vm_op_ldr(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t offset = sign_extend(instr & 0x3F, 6);
  vm->reg[r0] = vm_mem_read(vm, vm->reg[r1] + offset);
  vm_update_flags(vm, r0);
}

void vm_op_lea(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  vm->reg[r0] = vm->reg[R_PC] + pc_offset;
  vm_update_flags(vm, r0);
}

void vm_op_st(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  vm_mem_write(vm, vm->reg[R_PC] + pc_offset, vm->reg[r0]);
}

void vm_op_sti(VM *vm, uint16_t instr) {

  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  vm_mem_write(vm, vm_mem_read(vm, vm->reg[R_PC] + pc_offset), vm->reg[r0]);
}

void vm_op_str(VM *vm, uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t offset = sign_extend(instr & 0x3F, 6);
  vm_mem_write(vm, vm->reg[r1] + offset, vm->reg[r0]);
}

void vm_op_trap(VM *vm) { vm->reg[R_R7] = vm->reg[R_PC]; }

void vm_op_res(VM *vm) {
  log_message(LOG_LEVEL_ERROR,
              "Error: encountered reserved opcode (RES) at PC=0x%04X\n",
              vm->reg[R_PC] - 1);
  vm->running = false;
}

void vm_op_rti(VM *vm) {
  uint16_t sp = vm->reg[R_R6];
  vm->reg[R_PC] = vm_mem_read(vm, sp);
  vm->reg[R_R6] = sp + 1;
}

void vm_op_trap_getc(VM *vm) {
  vm->reg[R_R0] = (uint16_t)getchar();
  vm_update_flags(vm, R_R0);
}

void vm_op_trap_out(VM *vm) {
  putc((char)vm->reg[R_R0], stdout);
  fflush(stdout);
}

void vm_op_trap_puts(VM *vm) {
  uint16_t *c = vm->memory + vm->reg[R_R0];
  while (*c) {
    putc((char)*c, stdout);
    ++c;
  }
  fflush(stdout);
}

void vm_op_trap_in(VM *vm) {
  printf("Enter a character: ");
  char c = getchar();
  putc(c, stdout);
  fflush(stdout);
  vm->reg[R_R0] = (uint16_t)c;
  vm_update_flags(vm, R_R0);
}

void vm_op_trap_putsp(VM *vm) {
  /* one char per byte (two bytes per word)
     here we need to swap back to
     big endian format */
  uint16_t *c = vm->memory + vm->reg[R_R0];
  while (*c) {
    char char1 = (*c) & 0xFF;
    putc(char1, stdout);
    char char2 = (*c) >> 8;
    if (char2)
      putc(char2, stdout);
    ++c;
  }
  fflush(stdout);
}

void vm_op_trap_halt(VM *vm) {
  fprintf(stderr, "\n");
  log_message(LOG_LEVEL_INFO, "[VM HALT]\n");
  fflush(stdout);
  vm->running = false;
}
