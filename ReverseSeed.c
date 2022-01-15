/******************************/
/*    ReverseSeed.c by MAP    */
/******************************/

/* INCLUDE */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* TYPEDEF */

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/* DEFINE */

#define SEED_MAX_B (24) //hour
#define SEED_OFF_C (3600) //delay, 1 minute of leeway
#define STRING_LENGTH_MAX (16)

u8 Versions[2][STRING_LENGTH_MAX] = { "DPPT", "HGSS" };
u8 RngMethod[2][STRING_LENGTH_MAX] = { "Save & Quit", "New Game" };

/* Table for minimum delays depending on version and mode */
u16 MinimumDelays[2][2] = {
	{700, 5000}, //DPPT
	{550, 3000} //HGSS
};

void ReverseSeed(u32 seed, u8 version, u8 mode) {
	/* Find the nearest console-hitable seed provided the current state of the RNG. Print the number of iterations. */
	u16 min = MinimumDelays[version][mode];
	u16 frame = 0;
	u32 state = seed;
	u8 a = seed >> 24;
	u8 b = seed >> 16;
	u16 c = seed & 0xffff;
	/**/
	do {
		state = state * 0xEEB9EB65 + 0xA3561A1; //reverse
		a = state >> 24;
		b = state >> 16;
		c = state & 0xffff;
		frame++;
	} while (b > SEED_MAX_B || c < min || c > (min + SEED_OFF_C));
	/* Print result */
	printf("\n>Version           %s\n", Versions[version]);
	printf(">RNG Method        %s\n", RngMethod[mode]);
	printf(">Desired state     %08X\n", seed);
	printf(">Closest seed      %08X\n", state);
	printf(">Frame advances    %u\n", frame);
	printf("\n--------------------------------\n");
}

void ScanValue(u8 message[], u32 *value, u8 format[], u64 max) {
  /* General purpose safe scan. Instruction message, value to change, string format and max value */
  do {
    printf("%s", message);
    u8 userInput[STRING_LENGTH_MAX];
    fgets(userInput, STRING_LENGTH_MAX, stdin);
    if (strlen(userInput) == 0 || strlen(userInput) > STRING_LENGTH_MAX) {
      continue;
    }
    if (sscanf(userInput, format, value) != 1) {
      *value = max + 1;
      continue;
    }
  } while (*value > max);
}

int main() {

	u32 u_version;
	u32 u_mode;
	u32 u_state;

	while (1) {
		ScanValue("Game version (0=DPPT, 1=HGSS): ", &u_version, "%u", 1);
		ScanValue("Mode (0=Save&Quit, 1=NewGame): ", &u_mode, "%u", 1);
		ScanValue("Desired state to reach  (hex): ", &u_state, "%x", 0xffffffff);
		ReverseSeed(u_state, u_version, u_mode);
		printf("\n");
	}

	return 0;
}
