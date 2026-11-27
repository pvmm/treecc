// Function: Print_ValidatePattern
// Source: print.c

//-----------------------------------------------------------------------------
// Validate character. Try to convert invalid letter to their upper/lower case conterpart or use default invalid character
// @param		chr			Address of the character to check
// @param		patterns	Address of the font data to check
u8 Print_ValidatePattern(u8 chr, const c8** patterns)
{
	if ((chr < PRINT_DATA.CharFirst) || (chr > PRINT_DATA.CharLast))
	{
		if ((chr >= 'a') && (chr <= 'z') && (PRINT_DATA.CharFirst <= 'A') && (PRINT_DATA.CharLast >= 'Z')) // try to remap to upper case letter
		{
			chr = chr - 'a' + 'A';
			*patterns = PRINT_DATA.FontPatterns + PRINT_DATA.PatternY * (chr - PRINT_DATA.CharFirst);
		}
		else if ((chr >= 'A') && (chr <= 'Z') && (PRINT_DATA.CharFirst <= 'a') && (PRINT_DATA.CharLast >= 'z')) // try to remap to lower case letter
		{
			chr = chr - 'A' + 'a';
			*patterns = PRINT_DATA.FontPatterns + PRINT_DATA.PatternY * (chr - PRINT_DATA.CharFirst);
		}
		else
			*patterns = g_PrintInvalid;
	}
	return chr;
}
