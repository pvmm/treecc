// Function: Draw_Circle
// Header: draw.h
// Source: draw.c

// Function: Draw_Circle
// Draws a circle (one pixel wide)
//
// Parameters:
//   x - Destination position
//   y - Destination position
//   radius - Circle radius
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
void Draw_Circle(UX x, UY y, u8 radius, u8 color, u8 op);

//-----------------------------------------------------------------------------
// Draws a circle (one pixel wide)
// @author		Ibán Cereijo (https://ibancg.github.io/A-fast-circle-algorithm-for-ZX-Spectrum)
void Draw_Circle(UX dx, UY dy, u8 radius, u8 color, u8 op)
{
	USX x = radius;
	USY y = 0;
	USX e = 0;

	while (1)
	{
		// VDP_CommandPSET(dx + x, dy + y, color, op);
		g_VDP_Command.DX = dx + x;
		g_VDP_Command.DY = dy + y;
		g_VDP_Command.CLR = color;
		g_VDP_Command.ARG = 0;
		g_VDP_Command.CMD = VDP_CMD_PSET + op;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx + x, dy - y, color, op);
		g_VDP_Command.DY = dy - y;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx - x, dy - y, color, op);
		g_VDP_Command.DX = dx - x;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx - x, dy + y, color, op);
		g_VDP_Command.DY = dy + y;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx + y, dy + x, color, op);
		g_VDP_Command.DX = dx + y;
		g_VDP_Command.DY = dy + x;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx + y, dy - x, color, op);
		g_VDP_Command.DY = dy - x;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx - y, dy - x, color, op);
		g_VDP_Command.DX = dx - y;
		VDP_CommandSetupR36();
		// VDP_CommandPSET(dx - y, dy + x, color, op);
		g_VDP_Command.DY = dy + x;
		VDP_CommandSetupR36();

		if (x <= y)
			break;
		e += (2 * y) + 1;
		y++;
		if (e > x)
		{
			e += 1 - (2 * x);
			x--;
		}
	}
}
