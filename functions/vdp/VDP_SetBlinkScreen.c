// Function: VDP_SetBlinkScreen
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkScreen
// Set blink attribute on the whole screen. [MSX2/2+/TR]
inline void VDP_SetBlinkScreen() { VDP_FillVRAM(0xFF, g_ScreenColorLow, g_ScreenColorHigh, 80 * 27 / 8); }
