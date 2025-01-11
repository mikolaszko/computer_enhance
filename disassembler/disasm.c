#include <stdio.h>

char *regs16[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

char *regs8[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

char **regs[2] = {regs8, regs16};

int main(int argc, char *argv[]) {
  if (argc != 2)
    return printf("Please provide a file");

  char *filepath = argv[1];
  FILE *f = fopen(filepath, "rb");

  char b;

  // char b = 0x89;
  // b = 1000101
  // b >> 4 = 00001000
  // 0x0f = 00001111
  while (fread(&b, 1, 1, f)) {
    if (((b >> 4) & 0x0f) == 8) {
      // check if full or half
      int wide = b & 0x01;
      int to = (b >> 1) & 0x01;

      fread(&b, 1, 1, f);

      // ie. 11111111
      // bitmask on 00000011
      int mod = (b >> 6) & 0x03;
      int reg = (b >> 3) & 0x07;
      int rnm = b & 0x07;
      if (mod != 3)
        goto notdone;
      if (to == 1) {
        int tmp = reg;
        reg = rnm;
        rnm = tmp;
      }

      printf("mov %s, %s\n", regs[wide][rnm], regs[wide][reg]);
    } else {
      goto notdone;
    }
  }
  return 0;

notdone:
  printf("Opcode not implemented");
  return 0;
}
