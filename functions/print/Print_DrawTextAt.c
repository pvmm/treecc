// Function: Print_DrawTextAt
// Header: print.h
// Source: print.c

// Function: Print_DrawTextAt
// Draw text at a given position on screen.
//
// Parameters:
//   x     - Position X coordinate
//   y     - Position Y coordinate
//   str   - Null-terminated string to draw
inline void Print_DrawTextAt(UX x, UY y, const c8* str) { Print_SetPosition(x, y); Print_DrawText(str); }
