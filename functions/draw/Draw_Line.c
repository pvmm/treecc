// Function: Draw_Line
// Header: draw.h
// Source: draw.c

// Function: Draw_Line
// Draws a line (one pixel wide)
//
// Parameters:
//   x1 - Source position X
//   y1 - Source position Y
//   x2 - Destination position X
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a line (one pixel wide)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op)
{
	u16 dx, dy, nx, ny;
	u8 arg;

	arg = 0;
	if (x1 > x2)
	{
		arg |= VDP_ARG_DIX_LEFT;
		dx = x1 - x2 /*+ 1*/;
	}
	else // (x1 <= x2)
	{
		arg |= VDP_ARG_DIX_RIGHT;
		dx = x2 - x1 /*+ 1*/;
	}
	if (y1 > y2)
	{
		arg |= VDP_ARG_DIY_UP;
		dy = y1 - y2 /*+ 1*/;
	}
	else // (y1 <= y2)
	{
		arg |= VDP_ARG_DIY_DOWN;
		dy = y2 - y1 /*+ 1*/;
	}
	if (dx > dy)
	{
		arg |= VDP_ARG_MAJ_H;
		nx = dx;
		ny = dy;
	}
	else // (dx <= dy)
	{
		arg |= VDP_ARG_MAJ_V;
		nx = dy;
		ny = dx;
	}

	VDP_CommandLINE(x1, y1, nx, ny, color, arg, op);
}
