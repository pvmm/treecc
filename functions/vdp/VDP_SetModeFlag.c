// Function: VDP_SetModeFlag
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetModeFlag
// Set screen mode flag. [MSX1/2/2+/TR]
// VRAM tables address must be set to fit the new screen mode
//
// Parameters:
//   flag - Screen binary flag
void VDP_SetModeFlag(u8 flag);

//-----------------------------------------------------------------------------
// Set screen mode flag [MSX1/2/2+/TR]
// VRAM tables address must be set to fit the new screen mode
//
// Parameters:
//   flag - Screen binary flag
void VDP_SetModeFlag(u8 flag)
{
	// VDP R#1
	u8 reg1 = g_VDP_REGSAV[1];
	reg1 &= 0b11100111;
	if (flag & 0b00001)
		reg1 |= 0b00010000;
	if (flag & 0b00010)
		reg1 |= 0b00001000;
	VDP_RegWriteBak(1, reg1);

	// VDP R#0
	u8 reg0 = g_VDP_REGSAV[0];
	reg0 &= 0b11110001;
	flag >>= 1;
	flag &= 0b00001110;
	reg0 |= flag;
	VDP_RegWriteBak(0, reg0);
}
