// Function: String_Format
// Header: string.h
// Source: string.c

// Function: String_Format
// Build a zero-terminated string from a format string and variable number of parameters
// See <String_Format>
void String_FormatVA(c8* dest, const c8* format, va_list args);

//-----------------------------------------------------------------------------
// Build a zero-terminated string from a format string and variable number of parameters
void String_Format(c8* dest, const c8* format, ...)
{
	va_list args;
	va_start(args, format);
	String_FormatVA(dest, format, args);
	va_end(args);
}
