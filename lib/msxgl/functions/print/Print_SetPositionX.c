// Function: Print_SetPositionX
// Header: print.h
// Source: print.c

// Function: Print_SetPositionX
// Set cursor X coordinate.
//
// Parameters:
//   X - Print cursor position X coordinate
inline void Print_SetPositionX(UX x) { PRINT_DATA.CursorX = x; }
