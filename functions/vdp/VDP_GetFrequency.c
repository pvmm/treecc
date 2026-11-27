// Function: VDP_GetFrequency
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetFrequency
// GEt VDP frequency (R#9). [MSX2/2+/TR]
//
// Return:
//   Can be 50 (VDP_FREQ_50HZ) or 60 Hz (VDP_FREQ_60HZ)
inline u8 VDP_GetFrequency() { return g_VDP_REGSAV[9] & R09_NT; }
