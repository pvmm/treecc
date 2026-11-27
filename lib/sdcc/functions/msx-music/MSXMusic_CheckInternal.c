// Function: MSXMusic_CheckInternal
// Source: msx-music.c

//-----------------------------------------------------------------------------
//
bool MSXMusic_CheckInternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident;
	u16 dest = 0x4018;
	while (*ptr != 0)
	{
		if (*ptr != BIOS_InterSlotRead(slotId, dest++))
			return FALSE;
		ptr++;
	}
	return TRUE;
}
