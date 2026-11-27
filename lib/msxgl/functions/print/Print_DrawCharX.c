// Function: Print_DrawCharX
// Header: print.h
// Source: print.c

// Function: Print_DrawCharX
// Print the same character many times
//
// Parameters:
//   chr - ASCII code of the character to draw
//   num - Number of the same character to display in a row
void Print_DrawCharX(c8 chr, u8 num);

//-----------------------------------------------------------------------------
// Print the same character many times
// @param		chr			Character to draw
// @param		num			Number of drawing
void Print_DrawCharX(c8 chr, u8 num)
{
	for (u8 i = 0; i < num; ++i)
		Print_DrawChar(chr);
}
