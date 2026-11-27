// Function: String_FromUInt16
// Header: string.h
// Source: string.c

// Function: String_FromUInt16
// Create a string from a 16-bits unsigned integer (string is not zero-terminated)
// String buffer must be at least 5 bytes length (will be padded with '0')
// Original version by Milos "baze" Bazelides (http://map.grauw.nl/sources/external/z80bits.html#5.1)
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt16(u16 value, c8* string);
