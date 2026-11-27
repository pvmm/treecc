// Function: VDP_SetColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetColor
// Set text and border default color (R#7). [MSX1/2/2+/TR]
//
// Parameters:
//   color - Defaut color (Format: [TXT:4|BG:4])
inline void VDP_SetColor(u8 color) { VDP_RegWrite(7, color); }
