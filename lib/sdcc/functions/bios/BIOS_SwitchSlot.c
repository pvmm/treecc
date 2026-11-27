// Function: BIOS_SwitchSlot
// Header: bios.h
// Source: bios.c

// Function: BIOS_SwitchSlot
// Switches to specified slot and page definitively.
// Wrapper for ENASLT routine.
void BIOS_SwitchSlot(u8 page, u8 slot);

//-----------------------------------------------------------------------------
// ENASLT
// Address  : #0024
// Function : Selects the slot corresponding to the value of A and enables the slot to be used.
//            When this routine is called, interrupts are inhibited and remain so even after execution ends.
// Input    : A - Slot ID, see RDSLT
//            H - Bit 6 and 7 must contain the page number (00-11)
void BIOS_SwitchSlot(u8 page, u8 slot)
{
	page; // A
	slot; // L

	__asm
		LShift(6)					// A << 6
		ld		h, a
		ld		a, l
		call	R_ENASLT
		ei							// because ENASLT do DI
	__endasm;
}
