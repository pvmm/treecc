// Function: VDP_SetSpriteMultiColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteMultiColor
// Update sprite color (Multi-color). [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   ram   - Address to a 16 bytes buffer that determine the sprite colors (1 byte = 1 line).
//           With this function, you must provide a 16 bytes buffer even for 8x8 size mode.
void VDP_SetSpriteMultiColor(u8 index, const u8* ram);

//-----------------------------------------------------------------------------
// Update sprite color (Multi-color)
void VDP_SetSpriteMultiColor(u8 index, const u8* ram)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_WriteVRAM(ram, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);
}
