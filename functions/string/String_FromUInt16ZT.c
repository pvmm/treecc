// Function: String_FromUInt16ZT
// Header: string.h
// Source: string.c

// Function: String_FromUInt16ZT
// Create a zero-terminated string from a 16-bits unsigned integer
// String buffer must be at least 6 bytes length (will be padded with '0')
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt16ZT(u16 value, c8* string);
