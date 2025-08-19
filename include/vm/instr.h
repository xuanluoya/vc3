#ifndef VC3_VM_INSTR_H
#define VC3_VM_INSTR_H

#include <stdint.h>

#include "vm/vm.h"

typedef enum VMTrap {
  TRAP_GETC = 0x20,  /* Read a single character from keyboard (without echo) */
  TRAP_OUT = 0x21,   /* output a character */
  TRAP_PUTS = 0x22,  /* output a word string */
  TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
  TRAP_PUTSP = 0x24, /* output a byte string */
  TRAP_HALT = 0x25   /* halt the program */
} VMTrap;

/* Opcode enum */
typedef enum VMOpcode {
  OP_BR = 0,
  OP_ADD,
  OP_LD,
  OP_ST,
  OP_JSR,
  OP_AND,
  OP_LDR,
  OP_STR,
  OP_RTI,
  OP_NOT,
  OP_LDI,
  OP_STI,
  OP_JMP,
  OP_RES,
  OP_LEA,
  OP_TRAP
} VMOpcode;

/* VM instructions */
void vm_op_add(VM *vm, uint16_t instr);
void vm_op_and(VM *vm, uint16_t instr);
void vm_op_not(VM *vm, uint16_t instr);
void vm_op_br(VM *vm, uint16_t instr);
void vm_op_jmp(VM *vm, uint16_t instr);
void vm_op_jsr(VM *vm, uint16_t instr);
void vm_op_ld(VM *vm, uint16_t instr);
void vm_op_ldi(VM *vm, uint16_t instr);
void vm_op_ldr(VM *vm, uint16_t instr);
void vm_op_lea(VM *vm, uint16_t instr);
void vm_op_st(VM *vm, uint16_t instr);
void vm_op_sti(VM *vm, uint16_t instr);
void vm_op_str(VM *vm, uint16_t instr);
void vm_op_trap(VM *vm);
void vm_op_res(VM *vm);
void vm_op_rti(VM *vm);

/* VM Trap Routines instructions */
void vm_op_trap_getc(VM *vm);
void vm_op_trap_out(VM *vm);
void vm_op_trap_puts(VM *vm);
void vm_op_trap_in(VM *vm);
void vm_op_trap_putsp(VM *vm);
void vm_op_trap_halt(VM *vm);

#endif
