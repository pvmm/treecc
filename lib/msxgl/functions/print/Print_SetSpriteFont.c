// Function: Print_SetSpriteFont
// Header: print.h
// Source: print.c

// Function: Print_SetSpriteFont
// Set the current font and upload to Sprite Pattern Table.
// This function requires PRINT_USE_SPRITE compile option to be set to TRUE.
//
// Parameters:
//   font    - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   patIdx  - Pattern index to put font character
//   sprtIdx - Sprite index of the first sprite the module can use to display text
void Print_SetSpriteFont(const u8* font, u8 patIdx, u8 sprtIdx);

//-----------------------------------------------------------------------------
//
void Print_SetSpriteFont(const u8* font, u8 patIdx, u8 sprtIdx)
{
	PRINT_DATA.SpritePattern = patIdx;
	PRINT_DATA.SpriteID = sprtIdx;

	Print_SetFont(font);
	Print_Initialize();
	Print_SetMode(PRINT_MODE_SPRITE);

	#if (PRINT_HEIGHT == PRINT_HEIGHT_8)
		VDP_LoadSpritePattern(PRINT_DATA.FontPatterns, patIdx, PRINT_DATA.CharCount);
	#else // (PRINT_HEIGHT == PRINT_HEIGHT_X)
		u16 ram = (u16)PRINT_DATA.FontPatterns;
		u16 vram = g_SpritePatternLow;
		for (u16 chr = PRINT_DATA.CharFirst; chr <= PRINT_DATA.CharLast; ++chr)
		{
			u16 idx = chr - PRINT_DATA.CharFirst;
			if (PRINT_H(PRINT_DATA.PatternY) < 8)
			{
				VDP_WriteVRAM((u8*)ram, vram, g_SpritePatternHigh, PRINT_H(PRINT_DATA.PatternY));
				vram += PRINT_H(PRINT_DATA.PatternY);
				VDP_FillVRAM(0, vram, g_SpritePatternHigh, 8 - PRINT_H(PRINT_DATA.PatternY));
				vram += 8 - PRINT_H(PRINT_DATA.PatternY);
			}
			else
			{
				VDP_WriteVRAM((u8*)ram, vram, g_SpritePatternHigh, 8);
				vram += 8;
			}
			ram += PRINT_H(PRINT_DATA.PatternY);
		}
	#endif
}
