// Function: VDP_SetBlinkTile
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkTile
// Set blink attribute for a the given screen tile. [MSX2/2+/TR]
//
// Parameters:
//   x - Column number (0 to 79)
//   y - Row number (0 to 23, or 27 if <VDP_LINE_212> is set)
void VDP_SetBlinkTile(u8 x, u8 y);

//-----------------------------------------------------------------------------
// Set blink attribute for a the given tile
void VDP_SetBlinkTile(u8 x, u8 y)
{
	u16 addr = g_ScreenColorLow + (y * 10) + (x / 8);
	u8 chunk = VDP_Peek(addr, g_ScreenColorHigh);
	chunk |= g_VDPBitValue[7 - (x % 8)];
	VDP_Poke(chunk, addr, g_ScreenColorHigh);
}
