// Function: VDP_SetFrequency
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetFrequency
// Change VDP frequency (R#9). [MSX2/2+/TR]
//
// Parameters:
//   freq - Can be 50 (VDP_FREQ_50HZ) or 60 Hz (VDP_FREQ_60HZ)
inline void VDP_SetFrequency(u8 freq) { VDP_RegWriteBakMask(9, (u8)~R09_NT, freq); }
