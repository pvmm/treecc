// Function: VDP_SetSpritePositionX
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePositionX
// Update sprite position X. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31].
//   x     - Initial X screen coordinate.
void VDP_SetSpritePositionX(u8 index, u8 x);

//-----------------------------------------------------------------------------
// Update sprite position X
void VDP_SetSpritePositionX(u8 index, u8 x)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_Poke(x, ++low, g_SpriteAttributeHigh);
}
