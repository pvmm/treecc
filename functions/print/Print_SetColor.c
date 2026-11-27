// Function: Print_SetColor
// Header: print.h
// Source: print.c

// Function: Print_SetColor
// Set the draw color.
//
// Parameters:
//   text - Text color (16 color index or 8 bits RGB)
//   bg   - Background color (16 color index or 8 bits RGB)
void Print_SetColor(u8 text, u8 bg);

//-----------------------------------------------------------------------------
// Set the draw color
// @param		text		Text color (format depend of current screen mode)
// @param		bg			Background color (format depend of current screen mode)
void Print_SetColor(u8 text, u8 bg)
{
	if (VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))
			u8 t = text;
			#if (PRINT_USE_VALIDATOR)
				t = Print_SplitColor(t);
			#endif
			#if (PRINT_COLOR_NUM == 1)
				PRINT_DATA.TextColor = t;
			#else // if (PRINT_COLOR_NUM > 1)
				for (u8 i = 0; i < PRINT_COLOR_NUM; ++i)
				{
					PRINT_DATA.TextColor[i] = t;
				}
			#endif

			u8 b = bg;
			#if (PRINT_USE_VALIDATOR)
				b = Print_SplitColor(b);
			#endif
			PRINT_DATA.BGColor = b;

			Print_InitColorBuffer(t, b);
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			u8 col = text << 4 | bg;
			switch (VDP_GetMode())
			{
			#if (VDP_USE_MODE_T1)
				case VDP_MODE_TEXT1:		// 40 characters per line of text, one colour for each character
			#endif
			#if (VDP_USE_MODE_T2)
				case VDP_MODE_TEXT2:		// 80 characters per line of text, character blinkable selection
			#endif
			#if (VDP_USE_MODE_T1 || VDP_USE_MODE_T2)
					VDP_SetColor(col);
					break;
			#endif
			#if (VDP_USE_MODE_G1)
				case VDP_MODE_GRAPHIC1:		// 32 characters per one line of text, the COLOURed character available
					VDP_FillVRAM(col, g_ScreenColorLow, 0, 32);
					break;
			#endif
			#if (VDP_USE_MODE_G2)
				case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			#endif
			#if (VDP_USE_MODE_G3)
				case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			#endif
			#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
				{
					u16 dst = (u16)g_ScreenColorLow + PRINT_DATA.PatternOffset * 8;
					VDP_FillVRAM(col, dst, 0, PRINT_DATA.CharCount * 8);
					dst += 256 * 8;
					VDP_FillVRAM(col, dst, 0, PRINT_DATA.CharCount * 8);
					dst += 256 * 8;
					VDP_FillVRAM(col, dst, 0, PRINT_DATA.CharCount * 8);
					break;
				}
			#endif
			};
		#endif
	}
}
