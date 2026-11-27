// Function: VDP_GetMode
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetMode
// Get current screen mode. [MSX1/2/2+/TR]
//
// Return:
//   Current screen mode
inline u8 VDP_GetMode() { return g_VDP_Data.Mode; }
