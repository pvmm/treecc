// Function: Print_DrawText
// Header: print.h
// Source: print.c

// Function: Print_DrawText
// Print a character string
//
// Parameters:
//   string - Null-terminated string to draw
void Print_DrawText(const c8* string);

//-----------------------------------------------------------------------------
// Print a character string
// @param		chr			String to draw (must be NULL-terminated)
void Print_DrawText(const c8* str)
{
	while (*str != 0)
	{
		if (*str == '\t')
			Print_Tab();
		else if (*str == '\n')
			Print_Return();
	#if (PRINT_SKIP_SPACE)
		else if (*str == ' ')
			Print_Space();
	#endif
		else
			Print_DrawChar(*str);
		str++;
	}
}
