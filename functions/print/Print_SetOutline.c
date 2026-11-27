// Function: Print_SetOutline
// Header: print.h
// Source: print.c

// Function: Print_SetOutline
// Set shadow effect.
// This function requires PRINT_USE_FX_OUTLINE compile option to be set to TRUE.
//
// Parameters:
//   enable - TRUE to enable outline effect, FALSE to disable.
//   color  - Color of the outline effect
void Print_SetOutline(bool enable, u8 color);

//-----------------------------------------------------------------------------
// Set shadow effect
void Print_SetOutline(bool enable, u8 color)
{
	Print_EnableOutline(enable);
	PRINT_DATA.OutlineColor = color;
}
