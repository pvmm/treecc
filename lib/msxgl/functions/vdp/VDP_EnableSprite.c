// Function: VDP_EnableSprite
// Header: vdp.h
// Source: vdp.c

// Function: VDP_EnableSprite
// Enable/disable sprite rendering (R#8). [MSX2/2+/TR]
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_EnableSprite(u8 enable) { VDP_RegWriteBakMask(8, (u8)~R08_SPD, !enable ? R08_SPD : 0); }
