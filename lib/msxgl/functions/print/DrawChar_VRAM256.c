// Function: DrawChar_VRAM256
// Source: print.c

//-----------------------------------------------------------------------------
//
// @param		chr			The character to draw
void DrawChar_VRAM256(u8 chr)
{
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidateChar(chr);
	#endif
	u8 idx = chr - PRINT_DATA.CharFirst;
	u16 sx = (idx % PRINT_MOD256) * PRINT_W(PRINT_DATA.UnitX);
	u16 sy = (idx / PRINT_MOD256) * PRINT_H(PRINT_DATA.PatternY) + PRINT_DATA.FontVRAMY;

	if (PRINT_DATA.SourceMode == PRINT_MODE_BITMAP_VRAM)
		VDP_CommandHMMM(sx, sy, PRINT_DATA.CursorX, PRINT_DATA.CursorY, PRINT_W(PRINT_DATA.UnitX), PRINT_H(PRINT_DATA.PatternY));
	else
		VDP_CommandLMMM(sx, sy, PRINT_DATA.CursorX, PRINT_DATA.CursorY, PRINT_W(PRINT_DATA.UnitX), PRINT_H(PRINT_DATA.PatternY), VDP_OP_TIMP);
}
