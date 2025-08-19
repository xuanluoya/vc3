#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/log.h"
#include "core/util.h"
#include "vm/loader.h"

static void vm_load_from_file(VM *vm, FILE *file) {
  uint16_t origin;
  if (fread(&origin, sizeof(origin), 1, file) != 1) {
    log_message(LOG_LEVEL_ERROR, "Failed to read image origin");
    return;
  }
  origin = swap16(origin);

  log_message(LOG_LEVEL_DEBUG, "Loading image at 0x%04X", origin);

  if (origin != VM_PC_START) {
    log_message(LOG_LEVEL_WARN, "Non-standard origin 0x%04X (should be 0x%04X)",
                origin, VM_PC_START);
  }

  uint16_t max_read = VM_MEMORY_MAX - origin;
  if (max_read == 0) {
    log_message(LOG_LEVEL_ERROR, "Invalid origin address: 0x%04X", origin);
    return;
  }

  uint16_t *p = vm->memory + origin;
  size_t read = fread(p, sizeof(uint16_t), max_read, file);
  log_message(LOG_LEVEL_DEBUG, "Loaded %zu words", read);

  for (size_t i = 0; i < read; i++) {
    p[i] = swap16(p[i]);
  }
}

int vm_load_image(VM *vm, const char *image_path) {
  FILE *file = fopen(image_path, "rb");
  if (!file) {
    log_message(LOG_LEVEL_ERROR, "Failed to open image: %s", image_path);
    return 0;
  }

  vm_load_from_file(vm, file);
  fclose(file);

  log_message(LOG_LEVEL_INFO, "Loaded image: %s", image_path);
  return 1;
}
