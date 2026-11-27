// Function: Print_ValidateChar
// Source: print.c

//-----------------------------------------------------------------------------
// Validate character. Try to convert invalid letter to their upper/lower case conterpart or use default invalid character
// @param		chr			Address of the character to check
u8 Print_ValidateChar(u8 chr)
{
	if ((chr < PRINT_DATA.CharFirst) || (chr > PRINT_DATA.CharLast))
	{
		if ((chr >= 'a') && (chr <= 'z') && (PRINT_DATA.CharFirst <= 'A') && (PRINT_DATA.CharLast >= 'Z')) // try to remap to upper case letter
		{
			chr = chr - 'a' + 'A';
		}
		else if ((chr >= 'A') && (chr <= 'Z') && (PRINT_DATA.CharFirst <= 'a') && (PRINT_DATA.CharLast >= 'z')) // try to remap to lower case letter
		{
			chr = chr - 'A' + 'a';
		}
		else
			chr = PRINT_DATA.CharFirst;
	}
	return chr;
}
