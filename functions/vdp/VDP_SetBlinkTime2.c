// Function: VDP_SetBlinkTime2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkTime2
// Set blink time for the both defined colors (R#13). [MSX2/2+/TR]
// Check <VDP_BLINK_TIME> enumeration for correspondance between value and time in miliseconds
//
// Parameters:
//   even - Display time for even page / default color (0~15)
//   odd  - Display time for odd page / blink color (0~15)
inline void VDP_SetBlinkTime2(u8 even, u8 odd) { VDP_RegWrite(13, odd | (even << 4)); }
