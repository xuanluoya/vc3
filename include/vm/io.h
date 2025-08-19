#ifndef VC3_VM_IO_H
#define VC3_VM_IO_H

#include <stdint.h>

void vm_io_disable_input_buffering(void);
void vm_io_restore_input_buffering(void);
uint16_t vm_io_check_key(void);
void vm_io_handle_interrupt(int signal);

#endif
