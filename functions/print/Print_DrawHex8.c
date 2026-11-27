// Function: Print_DrawHex8
// Header: print.h
// Source: print.c

// Function: Print_DrawHex8
// Print a 8-bits binary value
//
// Parameters:
//   value - 8 bits integer to draw in hexadecimal form
void Print_DrawHex8(u8 value);

//-----------------------------------------------------------------------------
// Print a 8-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex8(u8 value)
{
	Print_DrawChar(g_HexChar[(value >> 4) & 0x000F]);
	Print_DrawChar(g_HexChar[value & 0x000F]);
	#if (PRINT_USE_UNIT)
		Print_DrawChar('h');
	#endif
}
