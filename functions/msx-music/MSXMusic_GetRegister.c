// Function: MSXMusic_GetRegister
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_GetRegister
// Get MSX-Music register value
//
// Parameters:
//   reg - Register to get
//
// Return:
//   Value of the register
u8 MSXMusic_GetRegister(u8 reg);

//-----------------------------------------------------------------------------
// Get MSX-Music register value (most of the registers are write-only)
u8 MSXMusic_GetRegister(u8 reg)
{
	io_MSXMusic_Reg = reg;
	return io_MSXMusic_Data;
}
