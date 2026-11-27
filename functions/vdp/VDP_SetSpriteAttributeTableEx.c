// Function: VDP_SetSpriteAttributeTableEx
// Header: vdp.h
// Source: vdp.c

// Function: VDP_SetSpriteAttributeTableEx
// Set sprite attribute table address. [MSX1/2/2+/TR]
inline void VDP_SetSpriteAttributeTableEx(VADDR addr, u8 r5, u8 r11) { g_SpriteAttributeLow = (u16)addr; VDP_RegWrite(5, r5); VADDR_14_CODE(r11;) VADDR_17_CODE(VDP_RegWrite(11, r11); g_SpriteAttributeHigh = addr >> 16;) addr -= 0x200; g_SpriteColorLow = (u16)addr; VADDR_17_CODE(g_SpriteColorHigh = addr >> 16;) }
