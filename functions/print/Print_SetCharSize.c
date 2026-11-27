// Function: Print_SetCharSize
// Header: print.h
// Source: print.c

// Function: Print_SetCharSize
// Set character size.
//
// Parameters:
//   X - Character width
//   Y - Character height
inline void Print_SetCharSize(u8 x, u8 y)
{
	PRINT_DATA.UnitX = x;
	PRINT_DATA.UnitY = y;
}
