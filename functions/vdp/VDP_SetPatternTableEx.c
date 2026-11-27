// Function: VDP_SetPatternTableEx
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetPatternTableEx
// Set pattern table VRAM address. [MSX1/2/2+/TR]
inline void VDP_SetPatternTableEx(VADDR addr, u8 r4) { g_ScreenPatternLow = (u16)addr; VDP_RegWrite(4, r4); VADDR_17_CODE(g_ScreenPatternHigh = addr >> 16;) }
