// Function: BIOS_InterSlotWrite
// Header: bios.h
// Source: bios.c

// Function: BIOS_InterSlotWrite
// Writes a value to an address in another slot.
// Wrapper for WRSLT routine.
void BIOS_InterSlotWrite(u8 slot, u16 addr, u8 value);

//-----------------------------------------------------------------------------
// WRSLT
// Address  : #0014
// Function : Writes a value to an address in another slot
// Input    : A  - ExxxSSPP  Slot-ID
//                 │   ││└┴─ Primary slot number (00-11)
//                 │   └┴─── Secondary slot number (00-11)
//                 └──────── Expanded slot (0 = no, 1 = yes)
//            HL - Address
//            E  - Value
// Registers: AF, BC, D
// Remark   : Can be call directly from MSX-DOS
//            This routine turns off the interrupt, but won't turn it on again
void BIOS_InterSlotWrite(u8 slot, u16 addr, u8 value)
{
	slot;  // A
	addr;  // DE
	value; // (SP+4)

	__asm
		push	iy
		ld		iy, #4
		add		iy, sp

		ld		l, e
		ld		h, d
		ld		e, 0(iy)
		call	R_WRSLT
		pop		iy
		ei
	__endasm;
}
