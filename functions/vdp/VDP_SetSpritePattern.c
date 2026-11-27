// Function: VDP_SetSpritePattern
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePattern
// Update sprite pattern. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31].
//   shape - Sprite pattern index (in 16x16 mode, only multiple of 4 can be used).
void VDP_SetSpritePattern(u8 index, u8 shape);

//-----------------------------------------------------------------------------
// Update sprite pattern
void VDP_SetSpritePattern(u8 index, u8 shape)
{
	u16 low = g_SpriteAttributeLow + 2;
	low += (index * 4);
	VDP_Poke(shape, low, g_SpriteAttributeHigh);
}
