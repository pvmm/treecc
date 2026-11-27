// Function: VDP_IsPatternMode
// Header: vdp.h
// Source: vdp.c

// Function: VDP_IsPatternMode
// Tell if the given screen mode is a pattern/text mode (bitmap mode otherwise). [MSX1/2/2+/TR]
//
// Parameters:
//   mode - VDP mode to set (see <VDP_MODE> enumeration)
//
// Return:
//   TRUE if the given screen mode is a pattern/text mode, FALSE otherwise.
inline bool VDP_IsPatternMode(const u8 mode) { return !VDP_IsBitmapMode(mode); }
