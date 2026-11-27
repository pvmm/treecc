// Function: VDP_SetBlinkColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkColor
// Set text and border blink color (R#12). [MSX2/2+/TR]
//
// Parameters:
//   color - Defaut color (Format: [TXT:4|BG:4])
inline void VDP_SetBlinkColor(u8 color) { VDP_RegWrite(12, color); }
