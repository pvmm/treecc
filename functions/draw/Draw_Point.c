// Function: Draw_Point
// Header: draw.h
// Source: draw.c

// Function: Draw_Point
// Draws a point
//
// Parameters:
//   x - Destination position
//   y - Destination position
//   color - Draw color (format depends on screen mode)
//   op - Draw operator (VDP_OP_AND, VDP_OP_OR, ...)
inline void Draw_Point(UX x, UY y, u8 color, u8 op) { VDP_CommandPSET(x, y, color, op); }
