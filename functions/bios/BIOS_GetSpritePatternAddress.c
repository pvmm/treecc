// Function: BIOS_GetSpritePatternAddress
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetSpritePatternAddress
// Returns the address of the sprite pattern table. Wrapper for CALPAT routine.
//
// Parameters:
//   id - Sprite pattern number [0:255] of 8x8 size and [0:63] of 16x16 size)
//
// Return:
//   VRAM address of the sprite pattern table for the given sprite number
u16 BIOS_GetSpritePatternAddress(u8 id);
