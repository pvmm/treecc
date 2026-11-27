// Function: VDP_SetVerticalOffset
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetVerticalOffset
// Set the vertical rendering offset (R#23). [MSX2/2+/TR]
//
// Parameters:
//   offset - Number of lines of offset from the top of the screen
inline void VDP_SetVerticalOffset(u8 offset) { VDP_RegWrite(23, offset); }
