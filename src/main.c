#include <stdio.h>
#include <stdlib.h>

#include "core/log.h"
#include "vm/loader.h"
#include "vm/vm.h"

int main(int argc, const char *argv[]) {

#ifdef DEBUG
  log_set_level(LOG_LEVEL_DEBUG);
  log_message(LOG_LEVEL_DEBUG, "Debug mode enabled");
#else
  log_set_level(LOG_LEVEL_INFO);
#endif

  VM vm;
  vm_init(&vm);

  for (int i = 1; i < argc; i++) {
    if (!vm_load_image(&vm, argv[i])) {
      log_message(LOG_LEVEL_ERROR, "Error loading image: %s\n", argv[i]);
      return EXIT_FAILURE;
    }
  }

  vm_run(&vm);

  return EXIT_SUCCESS;
}
