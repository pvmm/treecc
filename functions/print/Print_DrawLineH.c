// Function: Print_DrawLineH
// Header: print.h
// Source: print.c

// Function: Print_DrawLineH
// Draw an horizontal line using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x   - Start position X coordinate
//   y   - Start position Y coordinate
//   len - Lenght of the horizontal line (in print mode unit)
void Print_DrawLineH(UX x, UY y, u8 len);

//-----------------------------------------------------------------------------
// Draw an horizontal line using characters
void Print_DrawLineH(UX x, UY y, u8 len)
{
	const u8* style = g_PrintStyleChar[g_PrintDrawStyle];
	Print_SetPosition(x, y);
	Print_DrawCharX(style[PRINT_DRAW_CHR_H], len);
}
