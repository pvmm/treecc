// Function: Print_Tab
// Header: print.h
// Source: print.c

// Function: Print_Tab
// Print tabulation.
inline void Print_Tab()
{
	PRINT_DATA.CursorX += PRINT_W(PRINT_DATA.UnitX) + PRINT_DATA.TabSize - 1;
	PRINT_DATA.CursorX &= ~(PRINT_DATA.TabSize - 1);
}
