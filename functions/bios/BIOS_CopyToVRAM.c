// Function: BIOS_CopyToVRAM
// Header: bios.h
// Source: bios.c

// Function: BIOS_CopyToVRAM
// Block transfer to VRAM from memory. Wrapper for LDIRVM routine.
void BIOS_CopyToVRAM(const void* ram, u16 vram, u16 length);

//-----------------------------------------------------------------------------
// LDIRVM
// Address  : #005C
// Function : Block transfer from memory to VRAM
// Input    : HL - Start address of memory
//            DE - Start address of VRAM
//            BC - Block length
// Registers: All
// Remark   : In screen modes other than MSX1. The variable ACPAGE (0FAF6h) indicates the page from which the transfer must take place
void BIOS_CopyToVRAM(const void* ram, u16 vram, u16 length)
{
	ram;    // HL
	vram;   // DE
	length; // (SP+2)

	__asm
		ld		iy, #2
		add		iy, sp

		ld		c, 0(iy)
		ld		b, 1(iy)
		call	R_LDIRVM
	__endasm;
}
