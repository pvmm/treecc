// Function: VDP_EnableMask
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableMask
// Allows to hide the first 8 vertical lines at left of screen (R#25). [MSX2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableMask(u8 enable) { VDP_RegWriteBakMask(25, (u8)~R25_MAK, enable ? R25_MAK : 0); }
