// Function: VDP_SetInfiniteBlink
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetInfiniteBlink
// Set blink timer to always display alternative color (R#13). [MSX2/2+/TR]
inline void VDP_SetInfiniteBlink() { VDP_RegWrite(13, 0x10); }
