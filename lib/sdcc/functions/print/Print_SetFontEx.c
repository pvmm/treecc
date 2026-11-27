// Function: Print_SetFontEx
// Header: print.h
// Source: print.c

// Function: Print_SetFontEx
// Set the current font
//
// Parameters:
//   patternX - Width of the character data (should be always 8)
//   patternY - Height of the character data
//   sizeX    - Width of the chatacter to display (can be different than the one of the data)
//   sizeY    - Height of the chatacter to display (can be different than the one of the data)
//   firstChr - ASCII code of the first character included in the font data
//   lastChr  - ASCII code of the last character included in the font data
//   patterns - Address of the character data table
inline void Print_SetFontEx(u8 patternX, u8 patternY, u8 sizeX, u8 sizeY, u8 firstChr, u8 lastChr, const u8* patterns)
{
	PRINT_DATA.PatternX     = patternX;
	PRINT_DATA.PatternY     = patternY;
	PRINT_DATA.UnitX        = sizeX;
	PRINT_DATA.UnitY        = sizeY;
	PRINT_DATA.CharFirst    = firstChr;
	PRINT_DATA.CharLast     = lastChr;
	PRINT_DATA.CharCount    = lastChr - firstChr + 1;
	PRINT_DATA.FontPatterns = patterns;
	PRINT_DATA.FontAddr     = PRINT_DATA.FontPatterns - (firstChr * PRINT_DATA.PatternY); // pre-compute address of the virtual index 0 character (used to quick drawing in PutChar_GX functions)
}
