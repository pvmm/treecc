// Function: BIOS_WriteVDP
// Header: bios.h
// Source: bios.c

// Function: BIOS_WriteVDP
// Writes data in the VDP-register. Wrapper for WRTVDP routine.
void BIOS_WriteVDP(u8 reg, u8 value);

//-----------------------------------------------------------------------------
// WRTVDP
// Address  : #0047
// Function : Writes data in the VDP-register
// Input    : B  - Data to write
//            C  - Number of the register
// Registers: AF, BC
// Remark   : Call Sub-ROM on MSX2 or later if the bit EV of VDP register 0 is changed or if register number is greater than 7
void BIOS_WriteVDP(u8 reg, u8 value)
{
	reg;   // A
	value; // L

	__asm
		ld		c, a
		ld		b, l
		call	R_WRTVDP
	__endasm;
}
