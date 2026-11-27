// Function: Print_DrawHex8At
// Header: print.h
// Source: print.c

// Function: Print_DrawHex8At
// Print a 8-bits binary value at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   val - 8 bits integer to draw in hexadecimal form
inline void Print_DrawHex8At(UX x, UY y, u8 val) { Print_SetPosition(x, y); Print_DrawHex8(val); }
