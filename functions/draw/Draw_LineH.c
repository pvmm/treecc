// Function: Draw_LineH
// Header: draw.h
// Source: draw.c

// Function: Draw_LineH
// Draws an horizontal line (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   x2 - Destination position X
//   y - Position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a horizontal line (one pixel wide)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op)
{
	u16 dx, nx;
	if (x1 > x2)
	{
		dx = x2;
		nx = x1 - x2 + 1;
	}
	else
	{
		dx = x1;
		nx = x2 - x1 + 1;
	}
	VDP_CommandLMMV(dx, y, nx, 1, color, op);
}
