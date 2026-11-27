// Function: Print_SetFont
// Header: print.h
// Source: print.c

// Function: Print_SetFont
// Set the current font (and set mode to RAM).
//
// Parameters:
//   font - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
void Print_SetFont(const u8* font);

//-----------------------------------------------------------------------------
// Set the current font
// @param		font		Pointer to font data to use (NULL=use Main-ROM font)
void Print_SetFont(const u8* font)
{
	if (font == NULL) // Use Bios font (if any)
		Print_SetFontEx(8, 8, 6, 8, 1, 255, (const u8*)g_CGTABL + 8);
	else
		Print_SetFontEx(font[0] >> 4, font[0] & 0x0F, font[1] >> 4, font[1] & 0x0F, font[2], font[3], font+4);
}
