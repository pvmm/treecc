// Function: VDP_SetSpritePatternTableEx
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpritePatternTableEx
// Set sprite pattern table address. [MSX1/2/2+/TR]
inline void VDP_SetSpritePatternTableEx(VADDR addr, u8 r6) { g_SpritePatternLow  = (u16)addr; VDP_RegWrite(6, r6); VADDR_17_CODE(g_SpritePatternHigh = addr >> 16;) }
