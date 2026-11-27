// Function: BIOS_SetSpriteColor
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSpriteColor
// Set the color of a sprite.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   color - Sprite color [0:15]
inline void BIOS_SetSpriteColor(u8 id, u8 color) { BIOS_WriteVRAM(color, BIOS_GetSpriteAttributeAddress(id) + 3); }
