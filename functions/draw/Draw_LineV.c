// Function: Draw_LineV
// Header: draw.h
// Source: draw.c

// Function: Draw_LineV
// Draws a vertical line (one pixel wide)
//
// Parameters:
//   x - Position X
//   y1 - Source position Y
//   y2 - Destination position Y
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a vertical line (one pixel wide)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op)
{
	u16 dy, ny;
	if (y1 < y2)
	{
		dy = y1;
		ny = y2 - y1 + 1;
	}
	else
	{
		dy = y2;
		ny = y1 - y2 + 1;
	}
	VDP_CommandLMMV(x, dy, 1, ny, color, op);
}
