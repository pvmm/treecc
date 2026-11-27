// Function: VDP_EnableDisplay
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableDisplay
// Enable/disable screen display (R#1). [MSX1/2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableDisplay(bool enable) { VDP_RegWriteBakMask(1, (u8)~R01_BL, enable ? R01_BL : 0); }
