// Function: Char_IsAlpha
// Header: string.h
// Source: string.c

// Function: Char_IsAlpha
// Check if a given character is alphabetic (A-Za-z)
//
// Parameters:
//   chr - The character to check
//
// Return:
//   FALSE if the given character is not a alphabetic.
inline bool Char_IsAlpha(c8 chr)
{
	return ((chr >= 'A') && (chr <= 'Z')) || ((chr >= 'a') && (chr <= 'z'));
}
