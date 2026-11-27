// Function: VDP_SetSpritePosition
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePosition
// Update sprite position. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31].
//   x     - Initial X screen coordinate.
//   y     - Initial Y screen coordinate.
void VDP_SetSpritePosition(u8 index, u8 x, u8 y);

//-----------------------------------------------------------------------------
// Update sprite position
void VDP_SetSpritePosition(u8 index, u8 x, u8 y)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite

	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAttributeHigh, 2);
}
