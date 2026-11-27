// Function: VDP_SetFrameRender
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetFrameRender
// Set the frame render mode. [MSX2/2+/TR]
//
// Parameters:
//   mode - Can be VDP_FRAME_STATIC, VDP_FRAME_ALTERNANCE or VDP_FRAME_INTERLACE
inline void VDP_SetFrameRender(u8 mode) { VDP_RegWriteBakMask(9, (u8)~(R09_EO|R09_IL), mode); }
