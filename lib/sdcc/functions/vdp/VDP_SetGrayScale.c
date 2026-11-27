// Function: VDP_SetGrayScale
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetGrayScale
// Enable/disable grayscale (R#8). [MSX2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_SetGrayScale(bool enable) { VDP_RegWriteBakMask(8, (u8)~R08_BW, enable ? R08_BW : 0); }
