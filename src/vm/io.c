#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "vm/io.h"

struct termios original_tio;

void vm_io_disable_input_buffering(void) {
  tcgetattr(STDIN_FILENO, &original_tio);
  struct termios new_tio = original_tio;
  new_tio.c_lflag &= ~ICANON & ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void vm_io_restore_input_buffering(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

uint16_t vm_io_check_key(void) {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

void vm_io_handle_interrupt(int signal) {
  vm_io_restore_input_buffering();
  printf("\n");
  exit(-2);
}
