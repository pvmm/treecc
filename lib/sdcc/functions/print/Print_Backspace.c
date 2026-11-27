// Function: Print_Backspace
// Header: print.h
// Source: print.c

// Function: Print_Backspace
// Clear X character back from current cursor position.
//
// Parameters:
//   num - Number of character to clear
void Print_Backspace(u8 num);

//-----------------------------------------------------------------------------
// Clear X character back from current cursor position
// @param		num			Number of characters to remove
void Print_Backspace(u8 num)
{
	num;
	if (VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if (PRINT_USE_BITMAP)
			u16 x = PRINT_W(PRINT_DATA.UnitX) * num;
			if (x >  PRINT_DATA.CursorX)
				x = 0;
			else
				x = PRINT_DATA.CursorX - x;

			u8 color = Print_MergeColor(PRINT_DATA.BGColor);
			VDP_CommandHMMV(x, PRINT_DATA.CursorY, PRINT_DATA.CursorX - x, PRINT_H(PRINT_DATA.UnitY), color); // @todo Check the 192/212 lines parameter
			PRINT_DATA.CursorX = x;
			VDP_CommandWait();
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			u16 dst = g_ScreenLayoutLow + (PRINT_DATA.CursorY * PRINT_DATA.ScreenWidth) + PRINT_DATA.CursorX - num;
			VDP_FillVRAM(0, dst, g_ScreenLayoutHigh, num);
			PRINT_DATA.CursorX -= num;
		#endif
	}
}
