// Function: Print_SetPatternOffset
// Header: print.h
// Source: print.c

// Function: Print_SetPatternOffset
// Set pattern index where the font is stored.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
//
// Parameters:
//   offset - Pattern index where to start to store the font
inline void  Print_SetPatternOffset(u8 offset) { PRINT_DATA.PatternOffset = offset; }
