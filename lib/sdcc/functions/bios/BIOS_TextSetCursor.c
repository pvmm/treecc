// Function: BIOS_TextSetCursor
// Header: bios.h
// Source: bios.c

// Function: BIOS_TextSetCursor
// Moves cursor to the specified position. Wrapper for POSIT routine.
void BIOS_TextSetCursor(u8 x, u8 y);

//-----------------------------------------------------------------------------
// POSIT
// Address  : #00C6
// Function : Moves cursor to the specified position
// Input    : H  - X coordinate of cursor
//            L  - Y coordinate of cursor
// Registers: AF
void BIOS_TextSetCursor(u8 x, u8 y)
{
	x; // A
	y; // L

	__asm
		ld		h, a
		call	R_POSIT
	__endasm;
}
