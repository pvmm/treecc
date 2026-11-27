// Function: VDP_SetSprite
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSprite
// Set sprite attribute for Sprite Mode 2. [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   x     - Initial X screen coordinate
//   y     - Initial Y screen coordinate
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used)
void VDP_SetSprite(u8 index, u8 x, u8 y, u8 shape);

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2
void VDP_SetSprite(u8 index, u8 x, u8 y, u8 shape)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite
    g_VDP_Sprite.Pattern = shape;	// Pattern index

	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAttributeHigh, 3);
}
