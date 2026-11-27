// Function: Print_SetPositionY
// Header: print.h
// Source: print.c

// Function: Print_SetPositionY
// Set cursor Y coordinate.
//
// Parameters:
//   Y - Print cursor position Y coordinate
inline void Print_SetPositionY(UY y) { PRINT_DATA.CursorY = y; }
