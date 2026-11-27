// Function: VDP_SetYJK
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetYJK
// Set YJK mode. [MSX2+/TR].
// See <VDP_YJK> enum.
//
// Parameters:
//   mode - YJK mode (can be VDP_YJK_OFF, VDP_YJK_ON or VDP_YJK_YAE)
inline void VDP_SetYJK(u8 mode) { VDP_RegWriteBakMask(25, (u8)~VDP_YJK_YAE, mode); }
