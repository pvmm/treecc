// Function: MSXMusic_Mute
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_Mute
// Mute MSX-Music sound
void MSXMusic_Mute();

//-----------------------------------------------------------------------------
// Mute MSX-Music sound
void MSXMusic_Mute()
{
	MSXMUSIC_DOBACKUP(FALSE);
	loop (i, 9)
	{
		MSXMusic_SetRegister(MSXMUSIC_REG_CTRL_1 + i, 0); // seem to be enough
	}
	MSXMUSIC_DOBACKUP(TRUE);
}
