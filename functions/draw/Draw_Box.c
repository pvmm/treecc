// Function: Draw_Box
// Header: draw.h
// Source: draw.c

// Function: Draw_Box
// Draws a box (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a box (one pixel wide)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op)
{
	Draw_LineH(x1, x2, y1, color, op);
	Draw_LineH(x1, x2, y2, color, op);
	Draw_LineV(x1, y1, y2, color, op);
	Draw_LineV(x2, y1, y2, color, op);
}
