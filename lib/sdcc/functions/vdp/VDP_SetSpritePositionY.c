// Function: VDP_SetSpritePositionY
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePositionY
// Update sprite position Y. [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31].
//   y     - Initial Y screen coordinate.
void VDP_SetSpritePositionY(u8 index, u8 y);

//-----------------------------------------------------------------------------
// Update sprite position Y
void VDP_SetSpritePositionY(u8 index, u8 y)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_Poke(y, low, g_SpriteAttributeHigh);
}
