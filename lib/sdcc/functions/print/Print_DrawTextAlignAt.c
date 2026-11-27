// Function: Print_DrawTextAlignAt
// Header: print.h
// Source: print.c

// Function: Print_DrawTextAlignAt
// Draw text at a given position on screen.
//
// Parameters:
//   x     - Position X coordinate
//   y     - Position Y coordinate
//   str   - Null-terminated string to draw
//   align - Alignement flag: PRINT_ALIGN_LEFT, PRINT_ALIGN_CENTER, PRINT_ALIGN_RIGHT
inline void Print_DrawTextAlignAt(UX x, UY y, const c8* str, u8 align)
{
	Print_SetPosition(x, y);
	Print_DrawTextAlign(str, align);
}
