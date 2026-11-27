// Function: VDP_FastFillVRAM_16K
// Header: vdp.h
// Source: vdp.c

// Function: VDP_FastFillVRAM_16K
// Fill VRAM area with a given value [MSX1/2/2+/TR]
// Use ultra fast 16-bits using 16 unrolled out loop (~14,9 cc)
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destination address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Number of byte to copy in VRAM. Note: A count of 0 mean 65536
void VDP_FastFillVRAM_16K(u8 value, u16 dest, u16 count);
