// Function: VDP_EnableVBlank
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableVBlank
// Enable/disable vertical interruption (R#1). [MSX1/2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableVBlank(bool enable) { VDP_RegWriteBakMask(1, (u8)~R01_IE0, enable ? R01_IE0 : 0); }
