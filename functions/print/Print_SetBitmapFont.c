// Function: Print_SetBitmapFont
// Header: print.h
// Source: print.c

// Function: Print_SetBitmapFont
// Initialize print module and set a font in RAM.
// This function requires PRINT_USE_BITMAP compile option to be set to TRUE.
//
// Parameters:
//   font - Pointer to font structure. Character patterns plus a 4 bytes headers (character width/height, spacing width/height, 1st character code, last character code).
void Print_SetBitmapFont(const u8* font);

//-----------------------------------------------------------------------------
// Initialize print module. Must be called after VDP_SetMode()
// @param		font		Pointer to font data to use (NULL=use Main-ROM font)
void Print_SetBitmapFont(const u8* font)
{
	Print_SetFont(font);
	Print_SetMode(PRINT_MODE_BITMAP);
	Print_Initialize();
}
