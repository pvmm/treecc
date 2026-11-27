// Function: Print_DrawChar
// Header: print.h
// Source: print.c

// Function: Print_DrawChar
// Print a single character
//
// Parameters:
//   chr - ASCII code of the character to draw
void Print_DrawChar(u8 chr);

//-----------------------------------------------------------------------------
// Print a single character
// @param		chr			The character to draw
void Print_DrawChar(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		if (PRINT_DATA.CursorX + PRINT_W(PRINT_DATA.UnitX) > PRINT_DATA.ScreenWidth) // Handle automatic new-line when
			Print_Return();
		#if ((MSX_VERSION >= MSX_2) && (PRINT_USE_BITMAP))
			VDP_CommandWait();
		#endif
	#endif
	PRINT_DATA.DrawChar(chr);

	// PRINT_DATA.DrawChar(chr);
	PRINT_DATA.CursorX += PRINT_W(PRINT_DATA.UnitX);
}
