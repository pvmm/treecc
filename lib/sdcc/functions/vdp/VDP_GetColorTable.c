// Function: VDP_GetColorTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetColorTable
// Get address of the Color Table
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetColorTable() { return VADDR_GET(g_ScreenColorLow, g_ScreenColorHigh); }
