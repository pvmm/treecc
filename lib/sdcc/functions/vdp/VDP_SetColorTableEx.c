// Function: VDP_SetColorTableEx
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetColorTableEx
// Set color table VRAM address. [MSX1/2/2+/TR]
inline void VDP_SetColorTableEx(VADDR addr, u8 r3, u8 r10) { g_ScreenColorLow = (u16)addr; VDP_RegWrite(3, r3); VADDR_14_CODE(r10;) VADDR_17_CODE(VDP_RegWrite(10, r10); g_ScreenColorHigh = addr >> 16;) }
