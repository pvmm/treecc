// Function: BIOS_HideSprite
// Header: bios.h
// Source: bios.c

// Function: BIOS_HideSprite
// Hides a sprite from the screen (set vertical position to 212).
//
// Parameters:
//   id - Sprite attribute number [0:31]
inline void BIOS_HideSprite(u8 id) { u16 vram = BIOS_GetSpriteAttributeAddress(id); BIOS_WriteVRAM(212, vram); }
