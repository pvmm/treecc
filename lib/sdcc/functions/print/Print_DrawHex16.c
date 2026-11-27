// Function: Print_DrawHex16
// Header: print.h
// Source: print.c

// Function: Print_DrawHex16
// Print a 16-bits hexadecimal value
//
// Parameters:
//   value - 16 bits integer to draw in hexadecimal form
void Print_DrawHex16(u16 value);

//-----------------------------------------------------------------------------
// Print a 16-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex16(u16 value)
{
	Print_DrawHex8(value >> 8);
	Print_DrawHex8((u8)value);
}
