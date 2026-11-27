// Function: Sys_SetPageSlot
// Header: system.h
// Source: system.c

// Function: Sys_SetPageSlot
// Select a slot in a given page
//
// Parameters:
//   page - The page to change
//   slotId - The slot ID to select
void Sys_SetPageSlot(u8 page, u8 slotId);

//-----------------------------------------------------------------------------
// Set a slot in a given page
void Sys_SetPageSlot(u8 page, u8 slotId)
{
	page;
	slotId;
	// WORK IN PROGERSS...

	/*// static const u8 mask00[4] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
	// static const u8 mask01[4] = { 0b00000001, 0b00000100, 0b00010000, 0b01000000 };
	// static const u8 mask10[4] = { 0b00000010, 0b00001000, 0b00100000, 0b10000000 };
	// static const u8 mask11[4] = { 0b00000011, 0b00001100, 0b00110000, 0b11000000 };
	DisableInterrupt();
	if (slotId & SLOT_EXP)
	{
		// u8 prevSlot = io_PPI_Slot;
		// io_PPI_Slot = (prevSlot & ~(0b11000000 | (0b00000011 << (page * 2)))) | ((slotId & 0b00000011) << (page * 2)) | ((slotId & 0b00000011) << 6);
		// g_SLTSL = (~g_SLTSL & ~(0b00000011 << (page * 2))) | (((slotId & 0b00001100) >> 2) << (page * 2));

		u8 prevSlot = io_PPI_Slot;

	}
	else
	{
		io_PPI_Slot = (io_PPI_Slot & ~(0b00000011 << (page * 2))) | ((slotId & 0b00000011) << (page * 2));

		// u8 slot = *(mask00 + 4 * (slotId & 0b00000011) + page);
		// io_PPI_Slot = (io_PPI_Slot & ~mask11[page]) | (slot);
	}*/
}
