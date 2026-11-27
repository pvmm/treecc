// Function: VDP_SetSpriteUniColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteUniColor
// Update sprite color (Uni-color). [MSX2/2+/TR]
//
// Parameters:
//   index - Sprite index in the attribute table [0:31]
//   color - Sprite color (LSB 4-bits) + optional EC, CC & IC flags
//           (this color is used for the all 8 lines of the sprite)
void VDP_SetSpriteUniColor(u8 index, u8 color);

//-----------------------------------------------------------------------------
// Update sprite color (Uni-color)
void VDP_SetSpriteUniColor(u8 index, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);
}
