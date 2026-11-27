// Function: VDP_SetColor2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetColor2
// Set text and border default color (R#7). [MSX1/2/2+/TR]
//
// Parameters:
//   bg   - Background/border color (0~15)
//   text - Text color (0~15)
inline void VDP_SetColor2(u8 bg, u8 text) { VDP_RegWrite(7, bg | (text << 4)); }
