// Function: Print_DrawBox
// Header: print.h
// Source: print.c

// Function: Print_DrawBox
// Draw a box using characters
// This function requires PRINT_USE_GRAPH compile option to be set to TRUE.
//
// Parameters:
//   x      - Start position X coordinate
//   y      - Start position Y coordinate
//   width  - Box width (in print mode unit)
//   height - Box height (in print mode unit)
void Print_DrawBox(UX x, UY y, u8 width, u8 height);

//-----------------------------------------------------------------------------
// Draw a box using characters
void Print_DrawBox(UX x, UY y, u8 width, u8 height)
{
	const u8* style = g_PrintStyleChar[g_PrintDrawStyle];

	// Draw corners
	Print_SetPosition(x, y);
	Print_DrawChar(style[PRINT_DRAW_CHR_UL]);
	Print_SetPosition(x + width - 1, y);
	Print_DrawChar(style[PRINT_DRAW_CHR_UR]);
	Print_SetPosition(x, y + height - 1);
	Print_DrawChar(style[PRINT_DRAW_CHR_LL]);
	Print_SetPosition(x + width - 1, y + height - 1);
	Print_DrawChar(style[PRINT_DRAW_CHR_LR]);

	// Draw horizontal lines
	Print_DrawLineH(x + 1, y,              width - 2);
	Print_DrawLineH(x + 1, y + height - 1, width - 2);

	// Draw vertical lines
	Print_DrawLineV(x,             y + 1, height - 2);
	Print_DrawLineV(x + width - 1, y + 1, height - 2);
}
