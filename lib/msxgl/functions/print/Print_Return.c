// Function: Print_Return
// Header: print.h
// Source: print.c

// Function: Print_Return
// Print return.
inline void Print_Return()
{
	PRINT_DATA.CursorX = 0;
	PRINT_DATA.CursorY += PRINT_H(PRINT_DATA.UnitY);
}
