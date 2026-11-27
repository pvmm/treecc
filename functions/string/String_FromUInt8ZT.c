// Function: String_FromUInt8ZT
// Header: string.h
// Source: string.c

// Function: String_FromUInt8ZT
// Create a zero-terminated string from a 8-bits unsigned integer
// String buffer must be at least 4 bytes length (will be padded with '0')
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt8ZT(u8 value, c8* string);
