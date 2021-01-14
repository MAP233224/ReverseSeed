/******************************/
/*    ReverseSeed.c by MAP    */
/******************************/

/* INCLUDE */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/* TYPEDEF */

typedef unsigned __int64 u64;
typedef unsigned __int32 u32;
typedef unsigned __int16 u16;
typedef unsigned __int8 u8;

/* DEFINE */

#define SEED_MAX_B (24) //hour
#define SEED_OFF_C (3600) //delay, 1 minute of leeway

u16 VersionsModes[2][2] = {{700, 5000}, {550, 3000}}; //table for minimum delays depending on version and mode

void ReverseSeed(u32 seed, u8 ver, u8 mod) {
  /* Find the nearest console-hitable seed provided the current state of the RNG. Print the number of iterations. */
  u16 min = VersionsModes[ver][mod];
  u16 frame = 0;
  u32 state = seed;
  u8 a = (seed >> 24) & 0xff;
  u8 b = (seed >> 16) & 0xff;
  u16 c = seed & 0xffff;
  do {
    state = (state * 0xEEB9EB65 + 0xA3561A1) & 0xffffffff;
    a = (state >> 24) & 0xff;
    b = (state >> 16) & 0xff;
    c = state & 0xffff;
    frame += 1;
  } while (b > SEED_MAX_B || c < min || c > (min + SEED_OFF_C));
  printf("Closest you can hit on console is %08X (%d frames earlier).\n", state, frame);
}

u32 ScanValue(u8 message[], u32 value, u8 format[], u64 max) {
  do {
    printf("%s", message);
    u8 userInput[16];
    fgets(userInput, 9, stdin);
    if (strlen(userInput) == 0 || strlen(userInput) > 8) {
      continue;
    }
    if (sscanf(userInput, format, &value) != 1) {
      value = max+1;
      continue;
    }
  } while (value > max);
  return value;
}

int main() {
  while (1) {
    u32 version;
    u32 mode;
    u32 seed;
    version = ScanValue("Enter your game version (0=DPPT, 1=HGSS): ", version, "%u", 1);
    mode = ScanValue("Enter your mode (0=Save&Quit, 1=NewGame): ", mode, "%u", 1);
    seed = ScanValue("Enter the seed you want to reverse (hex): ", seed, "%x", 0xffffffff);
    ReverseSeed(seed, version, mode);
    printf("\n");
  }
  return 0;
}
