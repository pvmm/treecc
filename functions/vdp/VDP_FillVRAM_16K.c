// Function: VDP_FillVRAM_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_FillVRAM_16K
// Fill VRAM area with a given value. [MSX1/2/2+/TR]
// Use fast 16-bits loop (28 cc)
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destination address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Number of byte to copy in VRAM. Note: A count of 0 mean 65536
void VDP_FillVRAM_16K(u8 value, u16 dest, u16 count);
