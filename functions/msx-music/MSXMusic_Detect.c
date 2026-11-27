// Function: MSXMusic_Detect
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_Detect
// Search for MSX-Music (YM2413) chip
//
// Return:
//   MSXMUSIC_NOTFOUND if no MSX-Music chip found
//   MSXMUSIC_INTERNAL if an internal MSX-Music chip is found
//   MSXMUSIC_EXTERNAL if an external MSX-Music chip (FM-PAC)
u8 MSXMusic_Detect();

//-----------------------------------------------------------------------------
// Search for MSX-Music (YM2413) chip
u8 MSXMusic_Detect()
{
	// Check internal MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckInternal);
	if (g_MSXMusic_SlotId != SLOT_NOTFOUND)
		return MSXMUSIC_INTERNAL;

	// Check external MSX-Music chip
	g_MSXMusic_SlotId = Sys_CheckSlot(MSXMusic_CheckExternal);
	if (g_MSXMusic_SlotId != SLOT_NOTFOUND)
	{
		// Activate external FM-PAC
		u8 val = BIOS_InterSlotRead(g_MSXMusic_SlotId, 0x7FF6);
		BIOS_InterSlotWrite(g_MSXMusic_SlotId, 0x7FF6, val | 0x01);
		return MSXMUSIC_EXTERNAL;
	}

	return MSXMUSIC_NOTFOUND;
}
