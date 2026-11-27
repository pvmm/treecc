// Function: Print_DrawCharAt
// Header: print.h
// Source: print.c

// Function: Print_DrawCharAt
// Print a single character at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
inline void Print_DrawCharAt(UX x, UY y, c8 chr) { Print_SetPosition(x, y); Print_DrawChar(chr); }
