// Function: Print_SplitColor
// Source: print.c

// Slit color from merged colors
u8 Print_SplitColor(u8 color)
{
	#if (PRINT_USE_SPRITE)
	if (PRINT_DATA.SourceMode == PRINT_MODE_SPRITE)
		return color;
	#endif

	switch (VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4: return color & 0x0F;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5: return color & 0x03;
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6: return color & 0x0F;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7: return color;
	#endif
	}
	return color;
}
