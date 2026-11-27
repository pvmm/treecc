// Function: DrawChar_Layout
// Source: print.c

//-----------------------------------------------------------------------------
// Draw characters as pattern name
// @param		chr			The character to draw
void DrawChar_Layout(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidateChar(chr);
	#endif
	u8 shape = chr - PRINT_DATA.CharFirst + PRINT_DATA.PatternOffset;
	u16 dst = (u16)g_ScreenLayoutLow + (PRINT_DATA.CursorY * PRINT_DATA.ScreenWidth) + PRINT_DATA.CursorX;
	VDP_Poke(shape, dst, g_ScreenLayoutHigh);
}
