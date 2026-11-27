// Function: VDP_LoadBankColor_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_LoadBankColor_GM2
// Load tile colors in a given screen section (bank). [MSX1/2/2+/TR]
//
// Parameters:
//   src    - Address of data buffer to copy to VRAM
//   count  - Number of pattern to copy [1:255]
//   bank   - Screen section number (bank) from 0 (up) to 2 (bottom). On MSX2 or higher, a 4th bank is accessible via vertical scrolling
//   offset - Pattern index from which to copy data
inline void VDP_LoadBankColor_GM2(const u8* src, u8 count, u8 bank, u8 offset) { VDP_WriteVRAM(src, g_ScreenColorLow + (bank * 0x800) + (offset * 8), g_ScreenColorHigh, count * 8); }
