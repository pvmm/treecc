// Function: VDP_DisableSprite
// Header: vdp.h
// Source: vdp.c

// Function: VDP_DisableSprite
// Disable sprite rendering (see <VDP_EnableSprite>). [MSX2/2+/TR]
inline void VDP_DisableSprite() { VDP_EnableSprite(FALSE); }
