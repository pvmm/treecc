// Function: VDP_SetSpriteColorSM1
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteColorSM1
// Update sprite pattern (Sprite Mode 1). [MSX1/2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   color - Sprite color (LSB 4-bits) + optional EC flag (see <VDP_SPRITE_EC>)
void VDP_SetSpriteColorSM1(u8 index, u8 color);

//-----------------------------------------------------------------------------
// Update sprite pattern (Shader mode 1)
void VDP_SetSpriteColorSM1(u8 index, u8 color)
{
	u16 low = g_SpriteAttributeLow + 3;
	low += (index * 4);
	VDP_Poke(color, low, g_SpriteAttributeHigh);
}
