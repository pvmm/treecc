// Function: Draw_FillBox
// Header: draw.h
// Source: draw.c

// Function: Draw_FillBox
// Draws a fully filled box
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_FillBox(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a fully filled box
void Draw_FillBox(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op)
{
	VDP_CommandLMMV(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color, op);
}
