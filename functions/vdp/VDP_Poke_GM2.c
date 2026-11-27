// Function: VDP_Poke_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_Poke_GM2
// Set a GM cell value from x/y coordinate
inline void VDP_Poke_GM2(u8 x, u8 y, u8 value) { VDP_Poke(value, g_ScreenLayoutLow + (y * 32) + x, g_ScreenLayoutHigh); }
