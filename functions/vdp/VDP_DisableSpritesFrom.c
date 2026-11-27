// Function: VDP_DisableSpritesFrom
// Header: vdp.h
// Source: vdp.c

// Function: VDP_DisableSpritesFrom
// Disable all sprites from a given index. [MSX1/2/2+/TR]
//
// This function use VDP magic Y coordinate to disable the given sprite and all the following one.
// If the given sprite Y coordinate is modified to another value, it will be active again and so, the following sprites too.
//
// Parameters:
//   index - Disable sprite rendering starting from this sprite index
void VDP_DisableSpritesFrom(u8 index);

//-----------------------------------------------------------------------------
//
void VDP_DisableSpritesFrom(u8 index)
{
	u8 y = VDP_SPRITE_DISABLE_SM1;
	#if (MSX_VERSION >= MSX_2)
		if (g_VDP_Data.Mode >= VDP_MODE_MSX2) // MSX2 modes
			y = VDP_SPRITE_DISABLE_SM2;
	#endif
	VDP_SetSpritePositionY(index, y);
}
