// Function: MSXMusic_Initialize
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_Initialize
// Initialize MSX-Music module
//
// Return:
//   FALSE if not found
bool MSXMusic_Initialize();

//-----------------------------------------------------------------------------
// Initialize MSX-Music module
bool MSXMusic_Initialize()
{
	u8 ret = MSXMusic_Detect();
	#if (MSXMUSIC_USE_PAUSE)
	Mem_Set(0, g_MSXMusic_RegBackup, 16);
	#endif
	#if (MSXMUSIC_USE_CHANNEL)
	g_MSXMusic_ChanRhythm = 0;
	g_MSXMusic_ChanRhyTrig = 0;
	Mem_Set(0, g_MSXMusic_ChanKey, 9);
	Mem_Set(0, g_MSXMusic_ChanTrig, 9);
	#endif
	return ret != MSXMUSIC_NOTFOUND;
}
