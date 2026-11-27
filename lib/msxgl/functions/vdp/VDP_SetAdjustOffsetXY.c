// Function: VDP_SetAdjustOffsetXY
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetAdjustOffsetXY
// Adjustment of the display location on the screen (R#18). [MSX2/2+/TR]
//
// Parameters:
//   x - Horizontal screen display position offset [-7:+8]
//   y - Vertical screen display position offset [-7:+8]
inline void VDP_SetAdjustOffsetXY(i8 x, i8 y) { VDP_SetAdjustOffset(((-x) & 0x0F) | (((-y) & 0x0F) << 4)); }
