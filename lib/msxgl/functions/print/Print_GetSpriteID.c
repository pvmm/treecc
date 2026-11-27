// Function: Print_GetSpriteID
// Header: print.h
// Source: print.c

// Function: Print_GetSpriteID
// Get the next sprite index.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Return:
//   Sprite index of the first sprite the module can use to display text
inline u8 Print_GetSpriteID() { return PRINT_DATA.SpriteID; }
