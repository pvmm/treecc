// Function: VDP_SetSpriteExUniColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteExUniColor
// Set sprite attribute for Sprite Mode 2 and fill color table with unique color. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   x     - Initial X screen coordinate.
//   y     - Initial Y screen coordinate.
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used).
//   color - Sprite color (LSB 4-bits) + optional EC, CC & IC flags.
//           (this color is used for the all 8/16 lines of the sprite).
void VDP_SetSpriteExUniColor(u8 index, u8 x, u8 y, u8 shape, u8 color);

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2 and fill color table with unique color
void VDP_SetSpriteExUniColor(u8 index, u8 x, u8 y, u8 shape, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);

	g_VDP_Sprite.X = x;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
	g_VDP_Sprite.Y = y;				// X coordinate of the sprite
	g_VDP_Sprite.Pattern = shape;	// Pattern index
	u16 attr = g_SpriteAttributeLow;
	attr += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, attr, g_SpriteAttributeHigh, 3);
}
