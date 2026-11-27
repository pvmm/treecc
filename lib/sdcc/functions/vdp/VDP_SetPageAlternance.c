// Function: VDP_SetPageAlternance
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPageAlternance
// Enable automatic page switch on even/odd frames. [MSX2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_SetPageAlternance(bool enable) { VDP_RegWriteBakMask(9, (u8)~R09_EO, enable ? R09_EO : 0); }
