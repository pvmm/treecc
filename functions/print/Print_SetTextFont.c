// Function: Print_SetTextFont
// Header: print.h
// Source: print.c

// Function: Print_SetTextFont
// Initialize print module and set a font in RAM.
// This function requires PRINT_USE_TEXT compile option to be set to TRUE.
//
// Parameters:
//   font   - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
//   offset - Pattern index where to start to store the font
void Print_SetTextFont(const u8* font, u8 offset);

//-----------------------------------------------------------------------------
// Set the current font and upload it to VRAM
void Print_SetTextFont(const u8* fontData, u8 offset)
{
	PRINT_DATA.PatternOffset = offset;

	// Initialize font attributes
	if (fontData == NULL) // Use Bios font (if any)
		Print_SetFontEx(8, 8, 1, 1, 1, 255, (const u8*)g_CGTABL + 8); // @todo Should be [1, 255] to include all characters
	else
		Print_SetFontEx(8, 8, 1, 1, fontData[2], fontData[3], fontData+4);
	Print_Initialize();
	Print_SetMode(PRINT_MODE_TEXT);

	// Load font data to VRAM
	const u8* src = PRINT_DATA.FontPatterns;
	u16 dst = (u16)g_ScreenPatternLow + (offset * 8);
	#if (PRINT_USE_VALIDATOR)
		if (fontData != NULL)
			CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
		else
	#endif
		VDP_WriteVRAM(src, dst, 0, PRINT_DATA.CharCount * 8);

	switch (VDP_GetMode())
	{
	#if (VDP_USE_MODE_G2)
		case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
	#endif
	#if (VDP_USE_MODE_G3)
		case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
	#endif
	#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
		dst += 256 * 8;
		#if (PRINT_USE_VALIDATOR)
			if (fontData != NULL)
				CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
			else
		#endif
			VDP_WriteVRAM(src, dst, 0, PRINT_DATA.CharCount * 8);
		dst += 256 * 8;
		#if (PRINT_USE_VALIDATOR)
			if (fontData != NULL)
				CopyNo8HeightFontData(src, dst, fontData[0] & 0x0F);
			else
		#endif
			VDP_WriteVRAM(src, dst, 0, PRINT_DATA.CharCount * 8);
		break;
	#endif
	};
}
