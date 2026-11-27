// Function: String_Copy
// Header: string.h
// Source: string.c

// Function: String_Copy
// Copy a source string to a destination buffer
//
// Parameters:
//   dst - Destination string (buffer must be large enough to fit the source string size)
//   src - Source string
inline void String_Copy(c8* dst, const c8* src)
{
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}
