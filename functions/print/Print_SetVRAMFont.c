// Function: Print_SetVRAMFont
// Header: print.h
// Source: print.c

// Function: Print_SetVRAMFont
// Set the current font and upload data to VRAM .
// This function requires PRINT_USE_VRAM compile option to be set to TRUE.
//
// Parameters:
//   font  - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   y     - Y position to store the font in VRAM
//   color - Color of the font
//   trans - Use transparency
void Print_SetVRAMFont(const u8* font, UY y, u8 color, bool trans);

//-----------------------------------------------------------------------------
// Set the current font and upload it to VRAM
void Print_SetVRAMFont(const u8* font, UY y, u8 color, bool trans)
{
	UX cx = PRINT_DATA.CursorX;
	UY cy = PRINT_DATA.CursorY;

	Print_SetFont(font);
	Print_Initialize();
	Print_SetColor(color, 0x0);

	// Load font to VRAM
	Print_SetMode(PRINT_MODE_BITMAP_TRANS); // Activate default mode to write font data into VRAM
	PRINT_DATA.FontVRAMY = y;
	// @todo To optimize (pre-compute + fixed width/height cases
	u8 nx = PRINT_DATA.ScreenWidth / PRINT_W(PRINT_DATA.UnitX);
	for (u16 chr = PRINT_DATA.CharFirst; chr <= PRINT_DATA.CharLast; ++chr)
	{
		u16 idx = chr - PRINT_DATA.CharFirst;
		// @todo To optimize (pre-compute + fixed width/height cases
		PRINT_DATA.CursorX = (idx % nx) * PRINT_W(PRINT_DATA.UnitX);
		PRINT_DATA.CursorY = (idx / nx) * PRINT_H(PRINT_DATA.PatternY) + y;
		VDP_CommandHMMV(PRINT_DATA.CursorX, PRINT_DATA.CursorY, PRINT_W(PRINT_DATA.UnitX), PRINT_H(PRINT_DATA.PatternY), 0);
		PRINT_DATA.DrawChar(chr);
	}

	PRINT_DATA.CursorX = cx;
	PRINT_DATA.CursorY = cy;

	PRINT_DATA.CharPerLine = PRINT_DATA.ScreenWidth / PRINT_W(PRINT_DATA.UnitX);

	Print_SetMode(trans ? PRINT_MODE_BITMAP_VRAM_TRANS : PRINT_MODE_BITMAP_VRAM);
}
