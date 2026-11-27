// Function: Print_SetFontData
// Header: print.h
// Source: print.c

// Function: Print_SetFontData
// Set the current font data structure to use for print functions.
//
// Parameters:
//   data - Address of the font data structure to use for print functions.
inline void Print_SetFontData(Print_Data* data) { g_PrintData = data; }
