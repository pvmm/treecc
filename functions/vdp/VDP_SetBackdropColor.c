// Function: VDP_SetBackdropColor
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBackdropColor
// Set backdrop color (R#7). [MSX1/2/2+/TR]
//
// Parameters:
//   color - Backdrop color
inline void VDP_SetBackdropColor(u8 color) { VDP_SetColor(color); }
