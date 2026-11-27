// Function: VDP_GetSpriteAttributeTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetSpriteAttributeTable
// Get address of the Sprite Attribute Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpriteAttributeTable() { return VADDR_GET(g_SpriteAttributeLow, g_SpriteAttributeHigh); }
