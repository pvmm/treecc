// Function: VDP_GetSpritePatternTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetSpritePatternTable
// Get address of the Sprite Pattern Generator Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpritePatternTable() { return VADDR_GET(g_SpritePatternLow, g_SpritePatternHigh); }
