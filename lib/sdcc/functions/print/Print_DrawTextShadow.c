// Function: Print_DrawTextShadow
// Header: print.h
// Source: print.c

// Function: Print_DrawTextShadow
// Print a character string with shadow FX using 2 pass rendering (to have clean shadow over all characters).
// This function requires PRINT_USE_2_PASS_FX and PRINT_USE_FX_SHADOW compile options to be set to TRUE.
//
// Parameters:
//   string  - Pointer to the null-terminated string to display.
//   offsetX - Shadow position offset (X coordinate).
//   offsetY - Shadow position offset (X coordinate).
//   color   - Shadow color.
void Print_DrawTextShadow(const c8* string, i8 offsetX, i8 offsetY, u8 color);

//-----------------------------------------------------------------------------
// Print a character string using shadow FX.
void Print_DrawTextShadow(const c8* string, i8 offsetX, i8 offsetY, u8 color)
{
	// First pass
	UX x = PRINT_DATA.CursorX;
	UY y = PRINT_DATA.CursorY;
	Print_SetShadow(TRUE, offsetX, offsetY, color);
	PRINT_DATA.FX |= PRINT_FX_ONLY;
	Print_DrawText(string);
	// Second pass
	PRINT_DATA.FX = 0;
	Print_SetPosition(x, y);
	Print_DrawText(string);
	Print_EnableShadow(FALSE);
}
