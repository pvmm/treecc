// Function: BIOS_SetSpritePattern
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSpritePattern
// Set the pattern of a sprite.
//
// Parameters:
//   id  - Sprite attribute number [0:31]
//   pat - Sprite pattern number [0:255]
inline void BIOS_SetSpritePattern(u8 id, u8 pat) { BIOS_WriteVRAM(pat, BIOS_GetSpriteAttributeAddress(id) + 2); }
