// Function: Char_IsAlphaNum
// Header: string.h
// Source: string.c

// Function: Char_IsAlphaNum
// Check if a given character is alphanumeric (alphabet letter and numbers)
//
// Parameters:
//   chr - The character to check
//
// Return:
//   FALSE if the given character is not a alphabetic or numeric.
inline bool Char_IsAlphaNum(c8 chr)
{
	return Char_IsNum(chr) || Char_IsAlpha(chr);
}
