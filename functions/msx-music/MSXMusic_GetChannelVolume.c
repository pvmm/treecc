// Function: MSXMusic_GetChannelVolume
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_GetChannelVolume
// Get the current volume of a given channel (based on the values written with <MSXMusic_SetRegister>)
//
// Parameters:
//   chan - Channel index (0 to 8 for melodic channels, 9 to 13 for rhythm instruments)
//
// Return:
//   0 (off) or 15 (keyed on)
u8 MSXMusic_GetChannelVolume(u8 chan);

//-----------------------------------------------------------------------------
// Get the current volume of a given channel
u8 MSXMusic_GetChannelVolume(u8 chan)
{
	u8 s;
	if (chan < 9) // Melodic channel
	{
		if ((chan >= 6) && (g_MSXMusic_ChanRhythm & 0x20)) // Used by rhythm
			return 0;
		s = g_MSXMusic_ChanKey[chan] | g_MSXMusic_ChanTrig[chan];
		g_MSXMusic_ChanTrig[chan] = 0;
		return (s & 0x10) ? 15 : 0; // R#2x bit 4
	}
	// Rhythm instrument
	if (!(g_MSXMusic_ChanRhythm & 0x20)) // Rhythm mode
		return 0;
	u8 bit = 0x10 >> (chan - 9); // BD, SD, TOM, TCY, HH
	s = (g_MSXMusic_ChanRhythm | g_MSXMusic_ChanRhyTrig) & bit;
	g_MSXMusic_ChanRhyTrig &= ~bit;
	return s ? 15 : 0;
}
