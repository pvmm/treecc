// Function: BIOS_SetSpritePosition
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSpritePosition
// Set the position of a sprite on the screen.
//
// Parameters:
//   id - Sprite attribute number [0:31]
//   x  - Horizontal position of the sprite [0:255]
//   y  - Vertical position of the sprite [0:255]
inline void BIOS_SetSpritePosition(u8 id, u8 x, u8 y) { u16 vram = BIOS_GetSpriteAttributeAddress(id); BIOS_WriteVRAM(x, vram + 1); BIOS_WriteVRAM(y, vram); }
