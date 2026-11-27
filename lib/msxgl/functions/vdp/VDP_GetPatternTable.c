// Function: VDP_GetPatternTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetPatternTable
// Get address of the Pattern Generator Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetPatternTable() { return VADDR_GET(g_ScreenPatternLow, g_ScreenPatternHigh); }
