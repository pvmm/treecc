// Function: Print_DrawCharYAt
// Header: print.h
// Source: print.c

// Function: Print_DrawCharYAt
// Print vertically the same character many times at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   chr - Character to draw
//   len - Number of character to draw
inline void Print_DrawCharYAt(UX x, UY y, c8 chr, u8 len)
{
	for (u8 i = 0; i < len; ++i)
		Print_DrawCharAt(x, y++, chr);
}
