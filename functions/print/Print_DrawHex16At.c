// Function: Print_DrawHex16At
// Header: print.h
// Source: print.c

// Function: Print_DrawHex16At
// Print a 16-bits hexadecimal value at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   val - 16 bits integer to draw in hexadecimal form
inline void Print_DrawHex16At(UX x, UY y, u16 val) { Print_SetPosition(x, y); Print_DrawHex16(val); }
