// Function: BIOS_GetSpriteAttributeAddress
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetSpriteAttributeAddress
// Returns the address of the sprite attribute table. Wrapper for CALATR routine.
//
// Parameters:
//   id - Sprite attribute number [0:31]
//
// Return:
//   VRAM address of the sprite attribute table for the given sprite number
u16 BIOS_GetSpriteAttributeAddress(u8 id);
