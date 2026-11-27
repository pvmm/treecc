// Function: VDP_SetBlinkColor2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetBlinkColor2
// Set text and border blink color (R#12). [MSX2/2+/TR]
//
// Parameters:
//   bg   - Background/border color (0~15)
//   text - Text color (0~15)
inline void VDP_SetBlinkColor2(u8 bg, u8 text) { VDP_RegWrite(12, bg | (text << 4)); }
