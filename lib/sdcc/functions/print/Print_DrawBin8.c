// Function: Print_DrawBin8
// Header: print.h
// Source: print.c

// Function: Print_DrawBin8
// Print a 8-bits binary value
//
// Parameters:
//   value - 8 bits integer to draw in binary form (0 or 1)
void Print_DrawBin8(u8 value);

//-----------------------------------------------------------------------------
// Print a 8-bits binary value
// @param		value		Value to print
void Print_DrawBin8(u8 value)
{
	u8 flag = (u8)(1 << 7);
	for (u8 i = 0; i < 8; ++i)
	{
		Print_DrawChar((value & flag) ? '1' : '0');
		flag >>= 1;
	}
	#if (PRINT_USE_UNIT)
		Print_DrawChar('b');
	#endif
}
