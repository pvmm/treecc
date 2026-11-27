// Function: Print_DrawIntAt
// Header: print.h
// Source: print.c

// Function: Print_DrawIntAt
// Print a 16-bits signed decimal value at a given position.
//
// Parameters:
//   x   - Position X coordinate
//   y   - Position Y coordinate
//   val - 16 bits integer to display
inline void Print_DrawIntAt(UX x, UY y, i16 val) { Print_SetPosition(x, y); Print_DrawInt(val); }
