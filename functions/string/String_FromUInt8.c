// Function: String_FromUInt8
// Header: string.h
// Source: string.c

// Function: String_FromUInt8
// Create a string from a 8-bits unsigned integer (string is not zero-terminated)
// String buffer must be at least 3 bytes length (will be padded with '0')
// Original version derived from Galandros work (https://wikiti.brandonw.net/index.php?title=Z80_Routines:Other:DispA)
//
// Parameters:
//   value  - Integer value to convert
//   string - Destination string
void String_FromUInt8(u8 value, c8* string);
