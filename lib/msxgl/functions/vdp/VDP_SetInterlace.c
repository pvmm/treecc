// Function: VDP_SetInterlace
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetInterlace
// Enable or disable interlace mode. [MSX2/2+/TR]
// Needs <VDP_SetPageAlternance> to be set to TRUE and <VDP_SetPage> to a odd value.
//
// Parameters:
//   enable - TRUE to enable, FALSE to disable
inline void VDP_SetInterlace(bool enable) { VDP_RegWriteBakMask(9, (u8)~R09_IL, enable ? R09_IL : 0); }
