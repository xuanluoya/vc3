#ifndef VC3_VM_H
#define VC3_VM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Register enum */
typedef enum VMReg {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC,
  R_COND,
  R_COUNT
} VMReg;

/* Memory-Mapped Registers */
typedef enum VMMReg {
  MMR_KBSR = 0xfe00, /* keyboard status */
  MMR_KBDR = 0xfe02  /* keyboard data */
} VMMReg;

/* Condition flags */
typedef enum VMFlags {
  FL_POS = 1 << 0, /* Positive */
  FL_ZRO = 1 << 1, /* Zero */
  FL_NEG = 1 << 2  /* Negative */
} VMFlags;

/* Constants */
enum { VM_MEMORY_MAX = 1 << 16, VM_PC_START = 0x3000 };

/* VM Context */
typedef struct VM {
  uint16_t memory[VM_MEMORY_MAX];
  uint16_t reg[R_COUNT];
  bool running;
} VM;

/* VM */
void vm_init(VM *vm);
void vm_run(VM *vm);

#endif // LC3_VM_H
