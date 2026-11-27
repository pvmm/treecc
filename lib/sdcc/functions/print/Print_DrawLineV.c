// Function: Print_DrawLineV
// Header: print.h
// Source: print.c

// Function: Print_DrawLineV
// Draw a vertical line using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x   - Start position X coordinate
//   y   - Start position Y coordinate
//   len - Lenght of the vertical line (in print mode unit)
void Print_DrawLineV(UX x, UY y, u8 len);

//-----------------------------------------------------------------------------
// Draw a vertical line using characters
void Print_DrawLineV(UX x, UY y, u8 len)
{
	const u8* style = g_PrintStyleChar[g_PrintDrawStyle];
	for (u8 i = 0; i < len; i++)
	{
		Print_SetPosition(x, y + i);
		Print_DrawChar(style[PRINT_DRAW_CHR_V]);
	}
}
