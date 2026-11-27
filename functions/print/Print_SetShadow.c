// Function: Print_SetShadow
// Header: print.h
// Source: print.c

// Function: Print_SetShadow
// Set shadow effect.
// This function requires PRINT_USE_FX_SHADOW compile option to be set to TRUE.
//
// Parameters:
//   offsetX - Offset on X coordinate (can be negative).
//   offsetY - Offset on Y coordinate (can be negative).
//   color   - Shadow's color.
void Print_SetShadow(bool enable, i8 offsetX, i8 offsetY, u8 color);

//-----------------------------------------------------------------------------
// Set shadow effect
// @param		activate	Activate/deactivate shadow
// @param		offsetX		Shadow offset on X axis (can be from -3 to +4)
// @param		offsetY		Shadow offset on Y axis (can be from -3 to +4)
// @param		color		Shadow color (depend of the screen mode)
void Print_SetShadow(bool enable, i8 offsetX, i8 offsetY, u8 color)
{
	Print_EnableShadow(enable);
	PRINT_DATA.ShadowOffsetX = 3 + offsetX;
	PRINT_DATA.ShadowOffsetY = 3 + offsetY;
	PRINT_DATA.ShadowColor   = color;
}
