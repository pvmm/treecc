// Function: BIOS_FillVRAM
// Header: bios.h
// Source: bios.c

// Function: BIOS_FillVRAM
// Fills VRAM with value. Wrapper for FILVRM routine.
void BIOS_FillVRAM(u16 addr, u16 length, u8 value);

//-----------------------------------------------------------------------------
// FILVRM
// Address  : #0056
// Function : Fills the specified VRAM area with the same data
// Input    : HL - VRAM address to begin writing (00000h~03FFFh)
//            BC - The length of the area to be written
//            A  - Data byte
// Registers: AF, BC
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call BIGFIL (0016Bh)
void BIOS_FillVRAM(u16 addr, u16 length, u8 value)
{
	addr;   // HL
	length; // DE
	value;  // (SP+2)

	__asm
		ld		iy, #2
		add		iy, sp

		ld		c, e
		ld		b, d
		ld		a, 0(iy)
		call	R_FILVRM
	__endasm;
}
