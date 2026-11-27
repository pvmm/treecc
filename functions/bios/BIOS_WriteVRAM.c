// Function: BIOS_WriteVRAM
// Header: bios.h
// Source: bios.c

// Function: BIOS_WriteVRAM
// Writes data in VRAM. Wrapper for WRTVRM routine.
void BIOS_WriteVRAM(u8 value, u16 addr);

//-----------------------------------------------------------------------------
// WRTVRM
// Address  : #004D
// Function : Writes data in VRAM
// Input    : HL - VRAM address (00000h~03FFFh)
//            A  - Value write
// Registers: AF
// Remark   : Routine for MSX1, so only the 14 low order bits of the VRAM address are valid. To use all bits, call NWRVRM (00177h)
void BIOS_WriteVRAM(u8 value, u16 addr)
{
	value; // A
	addr;  // DE

	__asm
		ex		de, hl
		call	R_WRTVRM
	__endasm;
}
