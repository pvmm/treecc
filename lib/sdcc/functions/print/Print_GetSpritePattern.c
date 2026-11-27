// Function: Print_GetSpritePattern
// Header: print.h
// Source: print.c

// Function: Print_GetSpritePattern
// Get pattern index of the 1st sprite character.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Return:
//   Pattern index where the font is stored
inline u8 Print_GetSpritePattern() { return PRINT_DATA.SpritePattern; }
