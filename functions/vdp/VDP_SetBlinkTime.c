// Function: VDP_SetBlinkTime
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkTime
// Set blink time for the both defined colors (R#13). [MSX2/2+/TR]
//
// Parameters:
//   time - Display time (Format: [EVEN:4|ODD:4])
inline void VDP_SetBlinkTime(u8 time) { VDP_RegWrite(13, time); }
