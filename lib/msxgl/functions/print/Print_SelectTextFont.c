// Function: Print_SelectTextFont
// Header: print.h
// Source: print.c

// Function: Print_SelectTextFont
// Set print mode to text mode.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
// Parameters:
//   font   - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   offset - Pattern index where to start to store the font
inline void Print_SelectTextFont(const u8* font, u8 offset)
{
	PRINT_DATA.PatternOffset = offset;
	Print_SetFontEx(8, 8, 1, 1, font[2], font[3], font+4);
	// Print_Initialize();
	// Print_SetMode(PRINT_MODE_TEXT);
}
