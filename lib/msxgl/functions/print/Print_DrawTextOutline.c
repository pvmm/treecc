// Function: Print_DrawTextOutline
// Header: print.h
// Source: print.c

// Function: Print_DrawTextOutline
// Print a character string using outline FX.
// This function requires PRINT_USE_2_PASS_FX and PRINT_USE_FX_OUTLINE compile options to be set to TRUE.
//
// Parameters:
//   string  - Pointer to the null-terminated string to display
//   color   - Outline color
void Print_DrawTextOutline(const c8* string, u8 color);

//-----------------------------------------------------------------------------
// Print a character string using outline FX.
void Print_DrawTextOutline(const c8* string, u8 color)
{
	// First pass
	UX x = PRINT_DATA.CursorX;
	UY y = PRINT_DATA.CursorY;
	Print_SetOutline(TRUE, color);
	PRINT_DATA.FX |= PRINT_FX_ONLY;
	Print_DrawText(string);
	// Second pass
	PRINT_DATA.FX = 0;
	Print_SetPosition(x, y);
	Print_DrawText(string);
	Print_EnableShadow(FALSE);
}
