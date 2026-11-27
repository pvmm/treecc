// Function: VDP_Peek_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_Peek_GM2
// Get a GM cell value from x/y coordinate
inline u8 VDP_Peek_GM2(u8 x, u8 y) { return VDP_Peek(g_ScreenLayoutLow + (y * 32) + x, g_ScreenLayoutHigh); }
