// Function: VDP_SetBlinkChunkX
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkChunkX
// Set blink attribute for a the given number of chunk. [MSX2/2+/TR]
// A screen 'chunk' is a block of 8 consecutive tiles starting at a x-coordinate multiple of 8 column number (0, 8, 16, etc.)
//
// Parameters:
//   x - Column number (0 to 79)
//   y - Row number (0 to 23, or 27 if <VDP_LINE_212> is set)
inline void VDP_SetBlinkChunkX(u8 x, u8 y, u8 num) { VDP_FillVRAM(0xFF, g_ScreenColorLow + (y * 10) + (x / 8), g_ScreenColorHigh, num); }
