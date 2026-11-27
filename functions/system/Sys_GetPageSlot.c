// Function: Sys_GetPageSlot
// Header: system.h
// Source: system.c

// Function: Sys_GetPageSlot
// Get the slot ID of a given page
//
// Parameters:
//   page - The page to ckeck
//
// Return:
//   The slot ID
u8 Sys_GetPageSlot(u8 page);

//-----------------------------------------------------------------------------
// Get the slot ID of a given page
u8 Sys_GetPageSlot(u8 page)
{
	// Get page's primary slot
	u8 slot = (io_PPI_Slot >> (page * 2)) & 0x03;

	// Check if slot is expended
	if (g_EXPTBL[slot] & SLOT_EXP)
	{
		u8 prevSlot = io_PPI_Slot; // Backup current primary slots register
		DisableInterrupt();
		io_PPI_Slot = (prevSlot & 0x3F) | (slot << 6); // Select primary slot in page 3
		slot |= SLOT_EXP;
		slot |= (((~g_SLTSL) >> (page * 2)) & 0x03) << 2;
		io_PPI_Slot = prevSlot; // Restore primary slots register
		EnableInterrupt();
	}
	return slot;
}
