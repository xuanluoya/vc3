#include "core/util.h"

uint16_t sign_extend(uint16_t x, int bit_cont) {
  if ((x >> (bit_cont - 1)) & 1) {
    x |= (0xFFF << bit_cont);
  }

  return x;
}

uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }
