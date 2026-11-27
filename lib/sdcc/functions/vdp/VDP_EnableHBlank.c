// Function: VDP_EnableHBlank
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableHBlank
// Enable/disable horizontal interruption (R#0). [MSX2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableHBlank(bool enable) { VDP_RegWriteBakMask(0, (u8)~R00_IE1, enable ? R00_IE1 : 0); }
