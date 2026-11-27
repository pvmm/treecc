// Function: VDP_RegWriteBakMask
// Header: vdp.h
// Source: vdp.c

// Function: VDP_RegWriteBakMask
// Set register value after backuping previous value using mask. [MSX1/2/2+/TR]
//
// Parameters:
//   reg  - Register number
//   mask - Previous value reset mask
//   flag - Value to set
void VDP_RegWriteBakMask(u8 idx, u8 mask, u8 value);

//-----------------------------------------------------------------------------
// Set masked register value after backuping previous value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg  - Register number
//   mask - Previous value reset mask
//   flag - Value to set
void VDP_RegWriteBakMask(u8 reg, u8 mask, u8 flag)
{
	u8 value = g_VDP_REGSAV[reg];
	value &= mask;
	value |= flag;
	VDP_RegWriteBak(reg, value);
}
