// Function: Print_Initialize
// Header: print.h
// Source: print.c

// Function: Print_Initialize
// Initialize print module.
// Must be called after VDP_SetMode().
bool Print_Initialize();

//-----------------------------------------------------------------------------
// Initialize print module. Must be called after VDP_SetMode()
bool Print_Initialize()
{
	Print_SetColor(0xF, 0x0);
	Print_SetPosition(0, 0);
	#if (PRINT_USE_FX_SHADOW)
		Print_EnableShadow(FALSE);
	#endif
	#if (PRINT_USE_FX_OUTLINE)
		Print_EnableOutline(FALSE);
	#endif

	switch (VDP_GetMode()) // Screen mode specific initialization
	{
	#if (VDP_USE_MODE_T1)
		case VDP_MODE_TEXT1:		// 40 characters per line of text, one colour for each character
			Print_SetTabSize(3);
			PRINT_DATA.ScreenWidth = 40;
			break;
	#endif
	#if (VDP_USE_MODE_MC)
		case VDP_MODE_MULTICOLOR:	// pseudo-graphic, one character divided into four block
			break;
	#endif
	#if (VDP_USE_MODE_G1)
		case VDP_MODE_GRAPHIC1:		// 32 characters per one line of text, the COLOURed character available
			Print_SetTabSize(3);
			PRINT_DATA.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_G2)
		case VDP_MODE_GRAPHIC2:		// 256 x 192, the colour is specififed for each 8 dots
			Print_SetTabSize(3);
			PRINT_DATA.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_T2)
		case VDP_MODE_TEXT2:		// 80 characters per line of text, character blinkable selection
			Print_SetTabSize(3);
			PRINT_DATA.ScreenWidth = 80;
			break;
	#endif
	#if (VDP_USE_MODE_G3)
		#if (VDP_USE_UNDOCUMENTED)
		case VDP_MODE_GRAPHIC3_MIRROR_0:
		case VDP_MODE_GRAPHIC3_MIRROR_01:
		case VDP_MODE_GRAPHIC3_MIRROR_02:
		#endif
		case VDP_MODE_GRAPHIC3:		// GRAPHIC 2 which can use sprite mode 2
			Print_SetTabSize(3);
			PRINT_DATA.ScreenWidth = 32;
			break;
	#endif
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4:		// 256 x 212; 16 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			PRINT_DATA.ScreenWidth = 256;
			break;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5:		// 512 x 212; 4 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			Print_SetColor(0x03, 0x0);
			PRINT_DATA.ScreenWidth = 512;
			break;
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6:		// 512 x 212; 16 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			PRINT_DATA.ScreenWidth = 512;
			break;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7:		// 256 x 212; 256 colours are available for each dot
			Print_SetTabSize(PRINT_TAB_SIZE);
			Print_SetColor(0xFF, 0x0);
			PRINT_DATA.ScreenWidth = 256;
			break;
	#endif
	default:
		// Screen mode not (yet) supported!
		return FALSE;
	}

	// PRINT_DATA.PatternsPerLine = PRINT_DATA.ScreenWidth / PRINT_W(PRINT_DATA.UnitX);

	return TRUE;
}
