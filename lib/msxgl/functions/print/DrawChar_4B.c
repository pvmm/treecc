// Function: DrawChar_4B
// Source: print.c

//-----------------------------------------------------------------------------
// Graphic 4/6 (Screen mode 5/7) low-level function to draw a character in VRAM
// @param		chr			The character to draw
void DrawChar_4B(u8 chr)
{
	const u8* patterns = PRINT_DATA.FontAddr + chr * PRINT_H(PRINT_DATA.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	u8* l = (u8*)g_HeapStartAddress;
	for (u8 j = 0; j < PRINT_H(PRINT_DATA.PatternY); ++j) // Unpack each 6/8-bits line to buffer and send it to VRAM
	{
		#if (PRINT_COLOR_NUM > 1)
			Print_InitColorBuffer(PRINT_DATA.TextColor[j], PRINT_DATA.BGColor);
		#endif // (PRINT_COLOR_NUM > 1)
		u8 f = patterns[j];
		*l++ = PRINT_DATA.Buffer[f >> 6];
		*l++ = PRINT_DATA.Buffer[(f >> 4) & 0x03];
		*l++ = PRINT_DATA.Buffer[(f >> 2) & 0x03];
		#if (PRINT_WIDTH != PRINT_WIDTH_6)
			*l++ = PRINT_DATA.Buffer[f & 0x03];
		#endif
	}
	VDP_CommandHMMC((u8*)g_HeapStartAddress, PRINT_DATA.CursorX, PRINT_DATA.CursorY, DATA_LEN, PRINT_H(PRINT_DATA.PatternY));
}
