// Function: Print_SetColorShade
// Header: print.h
// Source: print.c

// Function: Print_SetColorShade
// Set colors shade.
// This function requires PRINT_COLOR_NUM compile option to be set to greater than 1 value.
//
// Parameters:
//   shade - Pointer to font color (1 color for each line)
void Print_SetColorShade(const u8* shade);

//-----------------------------------------------------------------------------
// Set color shade
void Print_SetColorShade(const u8* shade)
{
	if (VDP_IsBitmapMode(VDP_GetMode())) // Bitmap mode
	{
		#if ((PRINT_USE_BITMAP) || (PRINT_USE_SPRITE))
			for (u8 i = 0; i < PRINT_COLOR_NUM; ++i)
			{
				u8 t = shade[i];
				#if (PRINT_USE_VALIDATOR)
					t = Print_SplitColor(t);
				#endif
				PRINT_DATA.TextColor[i] = t;
			}
		#endif
	}
	else // Text mode
	{
		#if (PRINT_USE_TEXT)
			switch (VDP_GetMode())
			{
			#if (VDP_USE_MODE_G2)
				case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			#endif
			#if (VDP_USE_MODE_G3)
				case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			#endif
			#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)
				{
					u16 dst = (u16)g_ScreenColorLow + PRINT_DATA.PatternOffset * 8;
					for (u8 i = 0; i < PRINT_DATA.CharCount; ++i)
					{
						VDP_WriteVRAM(shade, dst,           0, 8);
						VDP_WriteVRAM(shade, dst + 256 * 8, 0, 8);
						VDP_WriteVRAM(shade, dst + 512 * 8, 0, 8);
						dst += 8;
					}
					break;
				}
			#endif
			};
		#endif
	}
}
