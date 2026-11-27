// Function: VDP_SetLineCount
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetLineCount
// Set line count for the current screen mode (R#9). [MSX2/2+/TR]
//
// Parameters:
//   lines - Can be VDP_LINE_192 or VDP_LINE_212
inline void VDP_SetLineCount(u8 lines) { VDP_RegWriteBakMask(9, (u8)~R09_LN, lines); }
