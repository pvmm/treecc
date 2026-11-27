// Function: BIOS_SetSprite
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSprite
// Set sprite attributes.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   x     - Horizontal position of the sprite [0:255]
//   y     - Vertical position of the sprite [0:255]
//   pat   - Sprite pattern number [0:255]
//   color - Sprite color [0:15]
inline void BIOS_SetSprite(u8 id, u8 x, u8 y, u8 pat, u8 color) { u16 vram = BIOS_GetSpriteAttributeAddress(id); BIOS_WriteVRAM(y, vram); BIOS_WriteVRAM(x, vram + 1); BIOS_WriteVRAM(pat, vram + 2); BIOS_WriteVRAM(color, vram + 3); }
