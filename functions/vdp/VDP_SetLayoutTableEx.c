// Function: VDP_SetLayoutTableEx
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetLayoutTableEx
// Set layout table VRAM address. [MSX1/2/2+/TR]
inline void VDP_SetLayoutTableEx(VADDR addr, u8 r2) { g_ScreenLayoutLow = (u16)addr; VDP_RegWrite(2, r2); VADDR_17_CODE(g_ScreenLayoutHigh = addr >> 16;) }
