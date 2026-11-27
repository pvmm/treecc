// Function: Print_EnableOutline
// Header: print.h
// Source: print.c

// Function: Print_EnableOutline
// Activate/desactivate shadow effect
// This function requires PRINT_USE_FX_OUTLINE compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable outline effect, FALSE to disable.
void Print_EnableOutline(bool enable);

//-----------------------------------------------------------------------------
// Activate/desactivate shadow effect
void Print_EnableOutline(bool enable)
{
	PRINT_DATA.FX = enable ? PRINT_FX_OUTLINE : 0;
	Print_SetMode(enable ? PRINT_MODE_BITMAP_TRANS : PRINT_MODE_BITMAP); // enable default mode to write font data into VRAM
}
