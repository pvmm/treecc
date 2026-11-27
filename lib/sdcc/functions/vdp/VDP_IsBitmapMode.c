// Function: VDP_IsBitmapMode
// Header: vdp.h
// Source: vdp.c

// Function: VDP_IsBitmapMode
// Tell if the given screen mode is a bitmap mode (pattern/text mode otherwise). [MSX1/2/2+/TR]
//
// Parameters:
//   mode - VDP mode to set (see <VDP_MODE> enumeration)
//
// Return:
//   TRUE if the given screen mode is a bitmap mode, FALSE otherwise.
inline bool VDP_IsBitmapMode(const u8 mode)
{
#if (MSX_VERSION == MSX_1)
	mode;
	return FALSE;
#else
	return mode >= VDP_MODE_GRAPHIC4;
#endif
}
