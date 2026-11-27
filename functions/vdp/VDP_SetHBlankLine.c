// Function: VDP_SetHBlankLine
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetHBlankLine
// Set the horizontal-blank interruption line (R#19). [MSX2/2+/TR]
//
// Parameters:
//   line - Line number where the interruption will occure
inline void VDP_SetHBlankLine(u8 line) { VDP_RegWrite(19, line); }
