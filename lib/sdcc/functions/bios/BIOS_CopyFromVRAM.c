// Function: BIOS_CopyFromVRAM
// Header: bios.h
// Source: bios.c

// Function: BIOS_CopyFromVRAM
// Block transfer to memory from VRAM. Wrapper for LDIRMV routine.
void BIOS_CopyFromVRAM(u16 vram, void* ram, u16 length);

//-----------------------------------------------------------------------------
// LDIRMV
// Address  : #0059
// Function : Block transfer from VRAM to memory
// Input    : HL - Start address of VRAM
//            DE - Start address of memory
//            BC - Block length
// Registers: All
// Remark   : In screen modes other than MSX1. The variable ACPAGE (0FAF6h) indicates the page from which the transfer must take place
void BIOS_CopyFromVRAM(u16 vram, void* ram, u16 length)
{
	vram;   // LH
	ram;    // DE
	length; // (SP+2)

	__asm
		ld		iy, #2
		add		iy, sp

		ld		c, 0(iy)
		ld		b, 1(iy)
		call	R_LDIRMV
	__endasm;
}
