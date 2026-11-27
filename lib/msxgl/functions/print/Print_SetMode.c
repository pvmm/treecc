// Function: Print_SetMode
// Header: print.h
// Source: print.c

// Function: Print_SetMode
// Change current print mode.
//
// Parameters:
//   mode - Print mode to select. Can be:
// > PRINT_MODE_BITMAP                Draw characters from RAM (R-T unpack font data and draw it). Need PRINT_USE_BITMAP or PRINT_USE_VRAM compile option.
// > PRINT_MODE_BITMAP_TRANS          Draw characters from RAM with transparency (R-T unpack font data and draw it). Need PRINT_USE_BITMAP or PRINT_USE_VRAM compile option.
// > PRINT_MODE_BITMAP_VRAM           Draw characters from VRAM (font data is upack once in VRAM thne drawing is done by VRAM copy). Need PRINT_USE_VRAM compile option.
// > PRINT_MODE_BITMAP_VRAM_TRANS     Draw characters from VRAM (font data is upack once in VRAM thne drawing is done by VRAM copy). Need PRINT_USE_VRAM compile option.
// > PRINT_MODE_SPRITE                Draw characters from sprites (load font data as sprite pattern in VRAM then display characters using sprite system). Need PRINT_USE_SPRITE compile option.
// > PRINT_MODE_TEXT                  Draw characters as pattern names (text mode). Need PRINT_USE_TEXT compile option.
void Print_SetMode(u8 mode);

//-----------------------------------------------------------------------------
//
void Print_SetMode(u8 mode)
{
	PRINT_DATA.SourceMode = mode;

	switch (PRINT_DATA.SourceMode)
	{
	#if (PRINT_USE_BITMAP)
		case PRINT_MODE_BITMAP:
		{
			switch (VDP_GetMode()) // Screen mode specific initialization
			{
			#if (VDP_USE_MODE_G5)
				case VDP_MODE_GRAPHIC5:
					PRINT_DATA.DrawChar = DrawChar_2B;
					break;
			#endif
			#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)
				case VDP_MODE_GRAPHIC4:
				case VDP_MODE_GRAPHIC6:
					PRINT_DATA.DrawChar = DrawChar_4B;
					break;
			#endif
			#if (VDP_USE_MODE_G7)
				case VDP_MODE_GRAPHIC7:
					PRINT_DATA.DrawChar = DrawChar_8B;
					break;
			#endif
			}
			break;
		}
		#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7)
		case PRINT_MODE_BITMAP_TRANS:
		{
			PRINT_DATA.DrawChar = DrawChar_Trans;
			break;
		}
		#endif
	#endif
	#if (PRINT_USE_VRAM)
		case PRINT_MODE_BITMAP_VRAM:
		case PRINT_MODE_BITMAP_VRAM_TRANS:
		{
			switch (VDP_GetMode()) // Screen mode specific initialization
			{
			#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G7)
				case VDP_MODE_GRAPHIC4:
				case VDP_MODE_GRAPHIC7:
					PRINT_DATA.DrawChar = DrawChar_VRAM256;
					break;
			#endif
			#if (VDP_USE_MODE_G5 || VDP_USE_MODE_G6)
				case VDP_MODE_GRAPHIC5:
				case VDP_MODE_GRAPHIC6:
					PRINT_DATA.DrawChar = DrawChar_VRAM512;
					break;
			#endif
			};
			break;
		}
	#endif
	#if (PRINT_USE_SPRITE)
		case PRINT_MODE_SPRITE:
		{
			PRINT_DATA.DrawChar = DrawChar_Sprite;
			break;
		}
	#endif
	#if (PRINT_USE_TEXT)
		case PRINT_MODE_TEXT:
		{
			PRINT_DATA.DrawChar = DrawChar_Layout;
			break;
		}
	#endif
	};
}
