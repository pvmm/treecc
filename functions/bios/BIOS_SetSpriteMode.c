// Function: BIOS_SetSpriteMode
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSpriteMode
// Set sprite size (8x8 or 16x16) and magnification mode (x1 or x2).
//
// Parameters:
//   size - Sprite size (BIOS_SPRITE_SIZE_8 or BIOS_SPRITE_SIZE_16)
//   mag - Sprite magnification (BIOS_SPRITE_MAG_1 or BIOS_SPRITE_MAG_2)
inline void BIOS_SetSpriteMode(u8 size, u8 mag) { BIOS_WriteVDP(1, (BIOS_ReadVDP(1) & 0xFC) | size | mag); }
