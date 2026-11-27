// Function: Print_DrawHex32
// Header: print.h
// Source: print.c

// Function: Print_DrawHex32
// Print a 32-bits hexadecimal value
// This function requires PRINT_USE_32B compile option to be set to TRUE.
//
// Parameters:
//   value - 32 bits integer to draw in hexadecimal form
void Print_DrawHex32(u32 value);

//-----------------------------------------------------------------------------
// Print a 16-bits hexadecimal value
// @param		value		Value to print
void Print_DrawHex32(u32 value)
{
	Print_DrawHex16(value >> 16);
	Print_DrawHex16((u16)value);
}
