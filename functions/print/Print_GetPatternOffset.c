// Function: Print_GetPatternOffset
// Header: print.h
// Source: print.c

// Function: Print_GetPatternOffset
// Get pattern index where the font is stored.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
// Return:
//   Pattern index where the font is stored
inline u8 Print_GetPatternOffset() { return PRINT_DATA.PatternOffset; }
