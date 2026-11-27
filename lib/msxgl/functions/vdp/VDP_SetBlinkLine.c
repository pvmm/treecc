// Function: VDP_SetBlinkLine
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkLine
// Set blink attribute on the given screen line. [MSX2/2+/TR]
//
// Parameters:
//   y - Line number (0 to 23, or 27 if <VDP_LINE_212> is set)
inline void VDP_SetBlinkLine(u8 y) { VDP_FillVRAM(0xFF, g_ScreenColorLow + y * 10, g_ScreenColorHigh, 10); }
