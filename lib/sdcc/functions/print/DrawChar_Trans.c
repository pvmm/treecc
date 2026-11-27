// Function: DrawChar_Trans
// Source: print.c

//-----------------------------------------------------------------------------
//
void DrawChar_Trans(u8 chr)
{
	const u8* patterns = PRINT_DATA.FontAddr + chr * PRINT_H(PRINT_DATA.PatternY); // Get character patterns' base address
	#if (PRINT_USE_VALIDATOR)
		chr = Print_ValidatePattern(chr, &patterns);
	#endif
	#if (PRINT_USE_FX_SHADOW)
		if (PRINT_DATA.FX & PRINT_FX_SHADOW)
		{
			g_VDP_Command.DY = PRINT_DATA.CursorY + PRINT_DATA.ShadowOffsetY - 3;
			g_VDP_Command.CLR = PRINT_DATA.ShadowColor;
			g_VDP_Command.ARG = 0;
			g_VDP_Command.CMD = VDP_CMD_PSET + 0;
			for (u8 j = 0; j < PRINT_H(PRINT_DATA.PatternY); ++j)
			{
				g_VDP_Command.DX = PRINT_DATA.CursorX + PRINT_DATA.ShadowOffsetX - 3;

				u8 f = patterns[j];
				if (f & BIT_7) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_6) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_5) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_4) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_3) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_2) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_1) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_0) VDP_CommandSetupR36();
				g_VDP_Command.DY++;
			}
		}
	#endif
	#if (PRINT_USE_FX_OUTLINE)
		if (PRINT_DATA.FX & PRINT_FX_OUTLINE)
		{
			g_VDP_Command.DY = PRINT_DATA.CursorY - 1;
			g_VDP_Command.NX = 3;
			g_VDP_Command.NY = 3;
			g_VDP_Command.CLR = PRINT_DATA.OutlineColor;
			g_VDP_Command.ARG = 0;
			g_VDP_Command.CMD = (u8)(VDP_CMD_LMMV + 0);
			for (u8 j = 0; j < PRINT_H(PRINT_DATA.PatternY); ++j)
			{
				g_VDP_Command.DX = PRINT_DATA.CursorX - 1;

				u8 f = patterns[j];
				if (f & BIT_7) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_6) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_5) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_4) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_3) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_2) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_1) VDP_CommandSetupR36(); g_VDP_Command.DX++;
				if (f & BIT_0) VDP_CommandSetupR36();
				g_VDP_Command.DY++;
			}
		}
	#endif
	#if (PRINT_USE_2_PASS_FX)
	if (PRINT_DATA.FX & PRINT_FX_ONLY)
		return;
	#endif

	g_VDP_Command.DY = PRINT_DATA.CursorY;
	g_VDP_Command.ARG = 0;
	g_VDP_Command.CMD = VDP_CMD_PSET + 0;
	#if (PRINT_COLOR_NUM == 1)
		g_VDP_Command.CLR = PRINT_DATA.TextColor;
	#endif
	for (u8 j = 0; j < PRINT_H(PRINT_DATA.PatternY); ++j)
	{
		#if (PRINT_COLOR_NUM > 1)
			g_VDP_Command.CLR = PRINT_DATA.TextColor[j];
		#endif
		g_VDP_Command.DX = PRINT_DATA.CursorX;

		u8 f = patterns[j];
		if (f & BIT_7) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_6) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_5) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_4) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_3) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_2) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_1) VDP_CommandSetupR36(); g_VDP_Command.DX++;
		if (f & BIT_0) VDP_CommandSetupR36();
		g_VDP_Command.DY++;
	}
}
