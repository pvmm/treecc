// Function: VDP_SetMode
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetMode
// Set screen mode. [MSX1/2/2+/TR]
//
// Parameters:
//   mode - The new screen mode to set (see <VDP_MODE> enumeration)
void VDP_SetMode(const u8 mode);

//-----------------------------------------------------------------------------
// Set screen mode [MSX1/2/2+/TR]
//
// Parameters:
//   mode - The new screen mode to set (see <VDP_MODE> enumeration)
void VDP_SetMode(const u8 mode)
{
	#if (VDP_AUTO_INIT)
	if (!g_VDPInitilized)
		VDP_Initialize();
	#endif

	#if (MSX_VERSION >= MSX_2P)
	VDP_RegWriteBak(25, 0);
	#endif

	g_VDP_Data.Mode = mode;
	switch (mode)
	{
//.............................................................................
// MSX1
#if (VDP_USE_MODE_T1)
	// case VDP_MODE_SCREEN0:
	// case VDP_MODE_SCREEN0_W40:
	case VDP_MODE_TEXT1:
		VDP_SetModeText1();
		break;
#endif

#if (VDP_USE_MODE_MC)
	// case VDP_MODE_SCREEN3:
	case VDP_MODE_MULTICOLOR:
		VDP_SetModeMultiColor();
		break;
#endif

#if (VDP_USE_MODE_G1)
	// case VDP_MODE_SCREEN1:
	case VDP_MODE_GRAPHIC1:
		VDP_SetModeGraphic1();
		break;
#endif

#if (VDP_USE_MODE_G2)
	// case VDP_MODE_SCREEN2:
	case VDP_MODE_GRAPHIC2:
		VDP_SetModeGraphic2();
		break;
#endif

//.............................................................................
// MSX2
#if (MSX_VERSION >= MSX_2)

#if (VDP_USE_MODE_T2)
	// case VDP_MODE_SCREEN0_W80:
	case VDP_MODE_TEXT2:
		VDP_SetModeText2();
		break;
#endif

#if (VDP_USE_MODE_G3)
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_0:
	case VDP_MODE_GRAPHIC3_MIRROR_01:
	case VDP_MODE_GRAPHIC3_MIRROR_02:
	#endif
	// case VDP_MODE_SCREEN4:
	case VDP_MODE_GRAPHIC3:
		VDP_SetModeGraphic3();
		break;
#endif


#if (VDP_USE_MODE_G4)
	// case VDP_MODE_SCREEN5:
	case VDP_MODE_GRAPHIC4:
	case VDP_MODE_SCREEN9_40: // @todo Further setting needed?
		VDP_SetModeGraphic4();
		break;
#endif

#if (VDP_USE_MODE_G5)
	// case VDP_MODE_SCREEN6:
	case VDP_MODE_GRAPHIC5:
	case VDP_MODE_SCREEN9_80: // @todo Further setting needed?
		VDP_SetModeGraphic5();
		break;
#endif

#if (VDP_USE_MODE_G6)
	// case VDP_MODE_SCREEN7:
	case VDP_MODE_GRAPHIC6:
		VDP_SetModeGraphic6();
		break;
#endif

#if (VDP_USE_MODE_G7)
	// case VDP_MODE_SCREEN8:
	case VDP_MODE_GRAPHIC7:
		VDP_SetModeGraphic7();
		#if (MSX_VERSION >= MSX_2P)
		VDP_SetYJK(VDP_YJK_OFF);
		#endif
		break;
#endif

#endif // (MSX_VERSION >= MSX_2)

//.............................................................................
// MSX2+
#if ((MSX_VERSION >= MSX_2P) && (VDP_USE_MODE_G7))
	case VDP_MODE_SCREEN10:
	case VDP_MODE_SCREEN11:
		VDP_SetModeGraphic7();
		VDP_SetYJK(VDP_YJK_YAE);
		break;

	case VDP_MODE_SCREEN12:
		VDP_SetModeGraphic7();
		VDP_SetYJK(VDP_YJK_ON);
		break;
#endif // ((MSX_VERSION >= MSX_2P) && (VDP_USE_MODE_G7))
	}

//.............................................................................
// Default VDP setting
#if (VDP_USE_DEFAULT_SETTINGS)
	VDP_EnableDisplay(TRUE);
	VDP_EnableVBlank(TRUE);
#if (!(MSX_VERSION & MSX_1))
	#if (VDP_INIT_50HZ == VDP_INIT_ON)
		VDP_SetFrequency(VDP_FREQ_50HZ);
	#elif (VDP_INIT_50HZ == VDP_INIT_OFF)
		VDP_SetFrequency(VDP_FREQ_60HZ);
	#elif (VDP_INIT_50HZ == VDP_INIT_AUTO)
		u8 freq = Sys_Is50Hz() ? VDP_FREQ_50HZ : VDP_FREQ_60HZ;
		VDP_SetFrequency(freq);
	#endif
	#if (VDP_USE_SPRITE)
		VDP_EnableSprite(TRUE);
	#else
		VDP_DisableSprite();
	#endif
	if (VDP_IsBitmapMode(mode)) // Activate 212 lines for bitmap mode
		VDP_SetLineCount(VDP_LINE_212);
	else
		VDP_SetLineCount(VDP_LINE_192);
	VDP_SetInterlace(FALSE);
	VDP_SetPageAlternance(FALSE);
#endif // (MSX_VERSION >= MSX_2)
#endif // (VDP_USE_DEFAULT_SETTINGS)
}
