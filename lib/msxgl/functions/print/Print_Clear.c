// Function: Print_Clear
// Header: print.h
// Source: print.c

// Function: Print_Clear
// Clear screen
void Print_Clear();

//-----------------------------------------------------------------------------
// Clear screen on the current page
void Print_Clear()
{
	if (VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if (PRINT_USE_BITMAP)
			u8 color = Print_MergeColor(PRINT_DATA.BGColor);
			VDP_CommandHMMV(0, 0, PRINT_DATA.ScreenWidth, 212, color); // @todo Check the 192/212 lines parameter
			VDP_CommandWait();
		#endif
	}
	#if (PRINT_USE_TEXT)
	else // Text mode
	{
		VDP_FillVRAM(0, g_ScreenLayoutLow, g_ScreenLayoutHigh, 24 * PRINT_DATA.ScreenWidth);
	}
	#endif
}
