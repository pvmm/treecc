// Function: BIOS_InterSlotCall
// Header: bios.h
// Source: bios.c

// Function: BIOS_InterSlotCall
// Executes inter-slot call.
// Wrapper for CALSLT routine.
void BIOS_InterSlotCall(u8 slot, u16 addr);

//-----------------------------------------------------------------------------
// CALSLT
// Address  : #001C
// Function : Executes inter-slot call.
// Input    : IY - High byte with slot ID, see RDSLT
//            IX - The address that will be called
// Remark   : Can be call directly from MSX-DOS
//            Variables can never be given in alternative registers or IX and IY
void BIOS_InterSlotCall(u8 slot, u16 addr)
{
	slot; // A
	addr; // DE
	__asm
		push	ix

		ld		l, #0
		ld		h, a
		push	hl
		pop		iy

		push	de
		pop		ix

		call	R_CALSLT

		ei							// because CALSLT do DI
		pop		ix
	__endasm;
}
