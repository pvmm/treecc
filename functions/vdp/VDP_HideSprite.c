// Function: VDP_HideSprite
// Header: vdp.h
// Source: vdp.c

// Function: VDP_HideSprite
// Hide a given sprite (don't disable following sprites). [MSX1/2/2+/TR]
//
// This function simply put the given sprite off (bellow) the screen.
//
// Parameters:
//   index - Index of the sprite to hide
inline void VDP_HideSprite(u8 index) { VDP_SetSpritePositionY(index, VDP_SPRITE_HIDE); }
