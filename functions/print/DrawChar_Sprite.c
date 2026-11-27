// Function: DrawChar_Sprite
// Source: print.c

//-----------------------------------------------------------------------------
//
// @param		chr			The character to draw
void DrawChar_Sprite(u8 chr)
{
	u16 shape = chr - PRINT_DATA.CharFirst + PRINT_DATA.SpritePattern;
	#if (PRINT_COLOR_NUM == 1)
		VDP_SetSpriteExUniColor(PRINT_DATA.SpriteID++, PRINT_DATA.CursorX, PRINT_DATA.CursorY, shape, PRINT_DATA.TextColor);
	#else // (PRINT_COLOR_NUM > 1)
		VDP_SetSpriteExMultiColor(PRINT_DATA.SpriteID++, PRINT_DATA.CursorX, PRINT_DATA.CursorY, shape, PRINT_DATA.TextColor);
	#endif
}
