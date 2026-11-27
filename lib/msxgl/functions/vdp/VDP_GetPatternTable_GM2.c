// Function: VDP_GetPatternTable_GM2
// Header: vdp.h
// Source: vdp.c

// Function: VDP_GetPatternTable_GM2
// Get address of the Pattern Generator Table of the given GM2 tilset bank (0-2)
//
// Return:
//   VRAM address of the table (u16 for 14-bits address and u32 for 17-bits)
inline VADDR VDP_GetPatternTable_GM2(u8 bank) { return VDP_GetPatternTable() + (bank * 2048); }
