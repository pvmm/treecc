// Function: BIOS_SetSpriteData
// Header: bios.h
// Source: bios.c

// Function: BIOS_SetSpriteData
// Set a sprite attributes.
//
// Parameters:
//   id    - Sprite attribute number [0:31]
//   attrs - Pointer to a 4-bytes array containing the sprite attributes (y, x, pattern and color)
inline void BIOS_SetSpriteData(u8 id, const BIOS_SpriteAttributes* attrs) { BIOS_CopyToVRAM(attrs, BIOS_GetSpriteAttributeAddress(id), 4); }
