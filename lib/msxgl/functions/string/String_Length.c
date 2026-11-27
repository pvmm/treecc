// Function: String_Length
// Header: string.h
// Source: string.c

// Function: String_Length
// Get the length of a given string
//
// Parameters:
//   str - The string to check
//
// Return:
//   String length.
inline u8 String_Length(const c8* str)
{
	u8 ret = 0;
	while (*str++)
		ret++;
	return ret;
}
