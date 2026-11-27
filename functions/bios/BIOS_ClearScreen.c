// Function: BIOS_ClearScreen
// Header: bios.h
// Source: bios.c

// Function: BIOS_ClearScreen
// Clears the screen. Wrapper for CLS routine.
void BIOS_ClearScreen();

//-----------------------------------------------------------------------------
// CLS
// Address  : #00C3
// Function : Clears the screen
// Registers: AF, BC, DE
// Remark   : Zero flag must be set to be able to run this routine
//            XOR A will do fine most of the time
void BIOS_ClearScreen()
{
	__asm
		xor		a, a
		call	R_CLS
	__endasm;
}
