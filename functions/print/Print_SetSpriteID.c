// Function: Print_SetSpriteID
// Header: print.h
// Source: print.c

// Function: Print_SetSpriteID
// Set the next sprite index.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Parameters:
//   id - Sprite index of the first sprite the module can use to display text
inline void Print_SetSpriteID(u8 id) { PRINT_DATA.SpriteID = id; }
