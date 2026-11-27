// Function: Print_DrawBin8At
// Header: print.h
// Source: print.c

// Function: Print_DrawBin8At
// Print a 8-bits binary value at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   val - 8 bits integer to draw in binary form (0 or 1)
inline void Print_DrawBin8At(UX x, UY y, u8 val) { Print_SetPosition(x, y); Print_DrawBin8(val); }
