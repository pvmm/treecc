// Function: Char_IsNum
// Header: string.h
// Source: string.c

// Function: Char_IsNum
// Check if a given character is numeric (0-9)
//
// Parameters:
//   chr - The character to check
//
// Return:
//   FALSE if the given character is not a numeric.
inline bool Char_IsNum(c8 chr)
{
	return (chr >= '0') && (chr <= '9');
}
