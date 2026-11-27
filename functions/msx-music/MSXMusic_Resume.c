// Function: MSXMusic_Resume
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_Resume
// Resume MSX-Music sound
void MSXMusic_Resume();

//-----------------------------------------------------------------------------
// Resume MSX-Music sound
void MSXMusic_Resume()
{
	loop (i, 9)
	{
		MSXMusic_SetRegister(MSXMUSIC_REG_CTRL_1 + i, g_MSXMusic_RegBackup[i]); // seem to be enough
	}
}
