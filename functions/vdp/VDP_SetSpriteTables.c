// Function: VDP_SetSpriteTables
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteTables
// Set sprite tables address. [MSX1/2/2+/TR]
//
// Parameters:
//   pattern - Sprite pattern table address
//   attrib	 - Sprite attribute table address
inline void VDP_SetSpriteTables(VADDR patAddr, VADDR attAddr) { VDP_SetSpritePatternTable(patAddr); VDP_SetSpriteAttributeTable(attAddr); }
