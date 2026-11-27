// Function: Print_DrawCharXAt
// Header: print.h
// Source: print.c

// Function: Print_DrawCharXAt
// Print the same character many times at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
//   len - Number of character to draw
inline void Print_DrawCharXAt(UX x, UY y, c8 chr, u8 len) { Print_SetPosition(x, y); Print_DrawCharX(chr, len); }
