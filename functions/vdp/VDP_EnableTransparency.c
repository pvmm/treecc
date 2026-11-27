// Function: VDP_EnableTransparency
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableTransparency
// Enable/disable color 0 transparency (R#8). [MSX2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableTransparency(u8 enable) { VDP_RegWriteBakMask(8, (u8)~R08_TP, !enable ? R08_TP : 0); }
