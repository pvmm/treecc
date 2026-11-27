// Function: VDP_GetSpriteColorTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetSpriteColorTable
// Get address of the Sprite Color Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetSpriteColorTable() { return VADDR_GET(g_SpriteColorLow, g_SpriteColorHigh); }
