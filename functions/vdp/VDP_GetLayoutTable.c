// Function: VDP_GetLayoutTable
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetLayoutTable
// Get address of the Pattern Layout Table (Name)
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetLayoutTable() { return VADDR_GET(g_ScreenLayoutLow, g_ScreenLayoutHigh); }
