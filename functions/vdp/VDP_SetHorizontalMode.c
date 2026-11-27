// Function: VDP_SetHorizontalMode
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetHorizontalMode
// Set horizontal scrolling to occurs on a single page or two pages (R#25). [MSX2+/TR]
//
// Parameters:
//   mode - Can be VDP_HSCROLL_SINGLE or VDP_HSCROLL_DOUBLE
inline void VDP_SetHorizontalMode(u8 mode) { VDP_RegWriteBakMask(25, (u8)~R25_SP2, mode); }
