// Function: VDP_SetBlinkChunkMask
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkChunkMask
// Set blink attribute mask for a the given screen chunk. [MSX2/2+/TR]
// A screen 'chunk' is a block of 8 consecutive tiles starting at a x-coordinate multiple of 8 column number (0, 8, 16, etc.)
//
// Parameters:
//   x    - Column number (0 to 79)
//   y    - Row number (0 to 23, or 27 if <VDP_LINE_212> is set)
//   mask - Chunk mask (1 bit is equal to 1 tile; bit #0 is the left most tile)
inline void VDP_SetBlinkChunkMask(u8 x, u8 y, u8 mask) { VDP_Poke(mask, g_ScreenColorLow + (y * 10) + (x / 8), g_ScreenColorHigh); }
