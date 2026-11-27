// Function: VDP_FillScreen_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_FillScreen_GM2
// Fill the full screen with a given pattern value. [MSX1/2/2+/TR]
//
// Parameters:
//   value - Pattern index to fill
//           In G2/G3 modes, the same index can define different pattern in the 3 vertical sections
inline void VDP_FillScreen_GM2(u8 value) { VDP_FillVRAM(value, g_ScreenLayoutLow, g_ScreenLayoutHigh, 32*24); }
