// Function: VDP_SetSpriteFlag
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteFlag
// Set sprite rendering parameters. [MSX1/2/2+/TR]
//
// Parameters:
//   flag - Sprite flag to be set. Can be a combination of:
//          VDP_SPRITE_SIZE_8 (Use 8x8 sprite size)
//          VDP_SPRITE_SIZE_16 (Use 16x16 sprite size)
//          VDP_SPRITE_SCALE_1 (Normal size of the sprite; 1 dot = 1 px)
//          VDP_SPRITE_SCALE_2 (Double the size of the sprite; 1 dot = 2 px)
inline void VDP_SetSpriteFlag(u8 flag) { VDP_RegWriteBakMask(1, (u8)~(R01_ST|R01_MAG), flag); }
