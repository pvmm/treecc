// Function: Print_SetPosition
// Header: print.h
// Source: print.c

// Function: Print_SetPosition
// Set cursor position.
//
// Parameters:
//   X - Print cursor position X coordinate
//   Y - Print cursor position Y coordinate
inline void Print_SetPosition(UX x, UY y)
{
	PRINT_DATA.CursorX = x;
	PRINT_DATA.CursorY = y;
}
