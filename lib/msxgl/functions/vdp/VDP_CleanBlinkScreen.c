// Function: VDP_CleanBlinkScreen
// Header: vdp.h
// Source: vdp.c

// Function: VDP_CleanBlinkScreen
// Clean blink attribute on the whole screen. [MSX2/2+/TR]
inline void VDP_CleanBlinkScreen() { VDP_FillVRAM(0x00, g_ScreenColorLow, g_ScreenColorHigh, 80 * 27 / 8); }
