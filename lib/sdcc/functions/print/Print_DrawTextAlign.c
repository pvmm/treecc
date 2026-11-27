// Function: Print_DrawTextAlign
// Header: print.h
// Source: print.c

// Function: Print_DrawTextAlign
// Print a character string
//
// Parameters:
//   str   - Null-terminated string to draw
//   align - Alignement flag: PRINT_ALIGN_LEFT, PRINT_ALIGN_CENTER, PRINT_ALIGN_RIGHT
inline void Print_DrawTextAlign(const c8* str, u8 align)
{
	u8 len = String_Length(str);
	switch (align)
	{
	case PRINT_ALIGN_LEFT:
		break;
	case PRINT_ALIGN_CENTER:
		PRINT_DATA.CursorX -= ((len - 1) / 2) * PRINT_W(PRINT_DATA.UnitX);
		break;
	case PRINT_ALIGN_RIGHT:
		PRINT_DATA.CursorX -= (len - 1) * PRINT_W(PRINT_DATA.UnitX);
		break;
	}

	Print_DrawText(str);
}
