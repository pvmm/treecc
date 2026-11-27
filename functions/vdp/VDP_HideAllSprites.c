// Function: VDP_HideAllSprites
// Header: vdp.h
// Source: vdp.c

// Function: VDP_HideAllSprites
// Hide all sprite sprite. [MSX1/2/2+/TR]
//
// This function simply put the sprites off (bellow) the screen.
//
// Parameters:
//   index - Index of the sprite to hide
inline void VDP_HideAllSprites() { loop (i, 32) VDP_SetSpritePositionY(i, VDP_SPRITE_HIDE); }
