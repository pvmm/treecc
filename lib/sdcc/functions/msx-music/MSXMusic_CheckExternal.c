// Function: MSXMusic_CheckExternal
// Source: msx-music.c

//-----------------------------------------------------------------------------
//
bool MSXMusic_CheckExternal(u8 slotId)
{
	const c8* ptr = g_MSXMusic_Ident + 4;
	u16 dest = 0x401C;
	while (*ptr != 0)
	{
		if (*ptr != BIOS_InterSlotRead(slotId, dest++))
			return FALSE;
		ptr++;
	}
	return TRUE;
}
