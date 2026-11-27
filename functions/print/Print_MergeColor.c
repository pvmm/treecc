// Function: Print_MergeColor
// Source: print.c

u8 Print_MergeColor(u8 color)
{
	switch (VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4: return (color & 0x0F) << 4 | (color & 0x0F);
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5: return (color & 0x03) << 6 | (color & 0x03) << 4 | (color & 0x03) << 2 | (color & 0x03);
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6: return (color & 0x0F) << 4 | (color & 0x0F);
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7: return color;
	#endif
	}
	return color;
}
