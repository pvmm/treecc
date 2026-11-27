// Function: MSXMusic_SetRegister
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_SetRegister
// Set MSX-Music register value
//
// Parameters:
//   reg   - Register to set
//   value - Value to set
void MSXMusic_SetRegister(u8 reg, u8 value);

//-----------------------------------------------------------------------------
// Set MSX-Music register value
void MSXMusic_SetRegister(u8 reg, u8 value)
{
	io_MSXMusic_Reg = reg;
	io_MSXMusic_Data = value;

	#if (MSXMUSIC_USE_PAUSE)
	if ((g_MSXMusic_DoBackup) && ((reg & 0xF0) == 0x20)) // MSXMUSIC_REG_CTRL_x
		g_MSXMusic_RegBackup[reg & 0x0F] = value;
	#endif

	#if (MSXMUSIC_USE_CHANNEL)
	u8 i = reg - MSXMUSIC_REG_CTRL_1;
	if (i < 9) // Key on
	{
		g_MSXMusic_ChanKey[i] = value;
		g_MSXMusic_ChanTrig[i] |= value;
	}
	else if (reg == MSXMUSIC_REG_RHYTHM)
	{
		g_MSXMusic_ChanRhythm = value;
		g_MSXMusic_ChanRhyTrig |= value;
	}
	#endif
}
