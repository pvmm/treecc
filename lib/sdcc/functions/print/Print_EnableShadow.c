// Function: Print_EnableShadow
// Header: print.h
// Source: print.c

// Function: Print_EnableShadow
// Activate/desactivate shadow effect.
// This function requires PRINT_USE_FX_SHADOW compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable shadow effect, FALSE to disable.
void Print_EnableShadow(bool enable);

//-----------------------------------------------------------------------------
// Activate/desactivate shadow effect
// @param		activate	Activate/deactivate shadow
void Print_EnableShadow(bool enable)
{
	PRINT_DATA.FX = enable ? PRINT_FX_SHADOW : 0;
	Print_SetMode(enable ? PRINT_MODE_BITMAP_TRANS : PRINT_MODE_BITMAP); // enable default mode to write font data into VRAM
}
