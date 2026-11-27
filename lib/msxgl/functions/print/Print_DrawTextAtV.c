// Function: Print_DrawTextAtV
// Header: print.h
// Source: print.c

// Function: Print_DrawTextAtV
// Draw a vertical text at a given position on screen.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   str - Null-terminated string to draw
inline void Print_DrawTextAtV(UX x, UY y, const c8* str)
{
	while (*str)
	{
		Print_SetPosition(x, y++);
		Print_DrawChar(*str++);
	}
}
