// Function: Print_SetDrawStyle
// Header: print.h
// Source: print.c

// Function: Print_SetDrawStyle
// Set the style of the graphic drawing functions
//
// Parameters:
//   style - Style to be selected
//> PRINT_DRAW_STYLE_LINE		Simple line
//> PRINT_DRAW_STYLE_DOUBLE		Double line
//> PRINT_DRAW_STYLE_DASHED		Dashed line
//> PRINT_DRAW_STYLE_PLAIN		Plain
inline void Print_SetDrawStyle(u8 style) { g_PrintDrawStyle = style; }
