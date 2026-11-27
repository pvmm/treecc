// Function: Print_InitColorBuffer
// Source: print.c

//-----------------------------------------------------------------------------
// Initialize color buffer
void Print_InitColorBuffer(u8 t, u8 b)
{
	// Pre-compute colors combinaison for 2-bits of a character pattern line (used to quick drawing in DrawChar_GX functions)
	switch (VDP_GetMode())
	{
	#if (VDP_USE_MODE_G4)
		case VDP_MODE_GRAPHIC4:
	#endif
	#if (VDP_USE_MODE_G6)
		case VDP_MODE_GRAPHIC6:
	#endif
	#if (VDP_USE_MODE_G4 || VDP_USE_MODE_G6)
			PRINT_DATA.Buffer[0] = (b << 4) | b;	// [ 0, 0 ]
			PRINT_DATA.Buffer[1] = (b << 4) | t;	// [ 0, 1 ]
			PRINT_DATA.Buffer[2] = (t << 4) | b;	// [ 1, 0 ]
			PRINT_DATA.Buffer[3] = (t << 4) | t;	// [ 1, 1 ]
			break;
	#endif
	#if (VDP_USE_MODE_G5)
		case VDP_MODE_GRAPHIC5:
			PRINT_DATA.Buffer[ 0] = (b << 6) | (b << 4) | (b << 2) | b;	// [ 0, 0, 0, 0 ]
			PRINT_DATA.Buffer[ 1] = (b << 6) | (b << 4) | (b << 2) | t;	// [ 0, 0, 0, 1 ]
			PRINT_DATA.Buffer[ 2] = (b << 6) | (b << 4) | (t << 2) | b;	// [ 0, 0, 1, 0 ]
			PRINT_DATA.Buffer[ 3] = (b << 6) | (b << 4) | (t << 2) | t;	// [ 0, 0, 1, 1 ]
			PRINT_DATA.Buffer[ 4] = (b << 6) | (t << 4) | (b << 2) | b;	// [ 0, 1, 0, 0 ]
			PRINT_DATA.Buffer[ 5] = (b << 6) | (t << 4) | (b << 2) | t;	// [ 0, 1, 0, 1 ]
			PRINT_DATA.Buffer[ 6] = (b << 6) | (t << 4) | (t << 2) | b;	// [ 0, 1, 1, 0 ]
			PRINT_DATA.Buffer[ 7] = (b << 6) | (t << 4) | (t << 2) | t;	// [ 0, 1, 1, 1 ]
			PRINT_DATA.Buffer[ 8] = (t << 6) | (b << 4) | (b << 2) | b;	// [ 1, 0, 0, 0 ]
			PRINT_DATA.Buffer[ 9] = (t << 6) | (b << 4) | (b << 2) | t;	// [ 1, 0, 0, 1 ]
			PRINT_DATA.Buffer[10] = (t << 6) | (b << 4) | (t << 2) | b;	// [ 1, 0, 1, 0 ]
			PRINT_DATA.Buffer[11] = (t << 6) | (b << 4) | (t << 2) | t;	// [ 1, 0, 1, 1 ]
			PRINT_DATA.Buffer[12] = (t << 6) | (t << 4) | (b << 2) | b;	// [ 1, 1, 0, 0 ]
			PRINT_DATA.Buffer[13] = (t << 6) | (t << 4) | (b << 2) | t;	// [ 1, 1, 0, 1 ]
			PRINT_DATA.Buffer[14] = (t << 6) | (t << 4) | (t << 2) | b;	// [ 1, 1, 1, 0 ]
			PRINT_DATA.Buffer[15] = (t << 6) | (t << 4) | (t << 2) | t;	// [ 1, 1, 1, 1 ]
			break;
	#endif
	#if (VDP_USE_MODE_G7)
		case VDP_MODE_GRAPHIC7:
			PRINT_DATA.Buffer[0] = b;	// [ 0, 0 ]
			PRINT_DATA.Buffer[1] = b;
			PRINT_DATA.Buffer[2] = b;	// [ 0, 1 ]
			PRINT_DATA.Buffer[3] = t;
			PRINT_DATA.Buffer[4] = t;	// [ 1, 0 ]
			PRINT_DATA.Buffer[5] = b;
			PRINT_DATA.Buffer[6] = t;	// [ 1, 1 ]
			PRINT_DATA.Buffer[7] = t;
			break;
	#endif
	}
}
