// Function: Print_DrawFormat
// Header: print.h
// Source: print.c

// Function: Print_DrawFormat
// Print a formatted string with a variable number of parameters.
// This function requires PRINT_USE_FORMAT compile option to be set to TRUE.
//
// Parameters:
//   format - Printf style format
// > %i or %d                  16 bits signed integer
// > %u                        16 bits unsigned integer
// > %[N]x                     16 bits hexadecimal with N digits (default: 4)
// > %c                        ASCII coded character
// > %s                        Null-terminated string
// > %I or %D                  32 bits signed integer
// > %U                        32 bits unsigned integer
// > %[N]X                     32 bits hexadecimal with N digits (default: 4)
// > \t                        Tabulation
// > \n                        Return to new line
//   ... - Variadic parameters
void Print_DrawFormat(const c8* format, ...);

//-----------------------------------------------------------------------------
// Print a formatted string with a variable number of parameters
void Print_DrawFormat(const c8* format, ...)
{
	// @todo To be replaced by String_Format() + Print_DrawText()

	va_list args;
	va_start(args, format);

	const c8* ptr = format;
	while (*ptr != 0)
	{
		if (*ptr == '%')
		{
			ptr++;

			// Parse length
			u8 len = 0;
			if ((*ptr >= '0') && (*ptr <= '9'))
			{
				len = *ptr - '0';
				ptr++;
			}
			while ((*ptr >= '0') && (*ptr <= '9'))
			{
				len *= 10;
				len += *ptr - '0';
				ptr++;
			}

			// Parse variable types
			if ((*ptr == 'i') || (*ptr == 'd'))
			{
				i16 val = (i16)va_arg(args, i16);
				Print_DrawInt(val);
			}
			else if (*ptr == 'u')
			{
				u16 val = (u16)va_arg(args, u16);
				Print_DrawInt(val);
			}
			else if (*ptr == 'x')
			{
				if (len == 0)
					len = 4;
				u16 val = (u16)va_arg(args, u16);
				if (len > 3)
					Print_DrawChar(g_HexChar[(val >> 12) & 0x000F]);
				if (len > 2)
					Print_DrawChar(g_HexChar[(val >> 8) & 0x000F]);
				if (len > 1)
					Print_DrawChar(g_HexChar[(val >> 4) & 0x000F]);
				Print_DrawChar(g_HexChar[val & 0x000F]);
			}
			else if (*ptr == 'b')
			{
				if (len == 0)
					len = 16;
				u16 val = (u16)va_arg(args, u16);
				u16 bit = 1 << (len - 1);
				while (bit)
				{
					if (val & bit)
						Print_DrawChar('1');
					else
						Print_DrawChar('0');
					bit >>= 1;
				}
			}
		#if (PRINT_USE_32B)
			else if ((*ptr == 'I') || (*ptr == 'D'))
			{
				i32 val = (i32)va_arg(args, i32);
				Print_DrawInt(val);
			}
			else if (*ptr == 'U')
			{
				u32 val = (u32)va_arg(args, u32);
				Print_DrawInt(val);
			}
			else if (*ptr == 'X')
			{
				if (len == 0)
					len = 8;
				u32 val = (u32)va_arg(args, u32);
				// Print_DrawHex32(val);
				if (len > 7)
					Print_DrawChar(g_HexChar[(val >> 28) & 0xF]);
				if (len > 6)
					Print_DrawChar(g_HexChar[(val >> 24) & 0xF]);
				if (len > 5)
					Print_DrawChar(g_HexChar[(val >> 20) & 0xF]);
				if (len > 4)
					Print_DrawChar(g_HexChar[(val >> 16) & 0xF]);
				if (len > 3)
					Print_DrawChar(g_HexChar[(val >> 12) & 0xF]);
				if (len > 2)
					Print_DrawChar(g_HexChar[(val >> 8) & 0xF]);
				if (len > 1)
					Print_DrawChar(g_HexChar[(val >> 4) & 0xF]);
				Print_DrawChar(g_HexChar[val & 0xF]);
			}
			else if (*ptr == 'B')
			{
				if (len == 0)
					len = 16;
				u32 val = (u32)va_arg(args, u32);
				u32 bit = 1 << (len - 1);
				while (bit)
				{
					if (val & bit)
						Print_DrawChar('1');
					else
						Print_DrawChar('0');
					bit >>= 1;
				}
			}
		#endif
			else if (*ptr == 'c')
			{
				c8 val = (c8)va_arg(args, u16);
				Print_DrawChar(val);
			}
			else if (*ptr == 's')
			{
				const c8* val = (const c8*)va_arg(args, const c8*);
				Print_DrawText(val);
			}
			else if (*ptr == '%')
			{
				Print_DrawChar('%');
			}
		}
		// Parse special character
		else if (*ptr == '\t')
			Print_Tab();
		else if (*ptr == '\n')
			Print_Return();
	#if (PRINT_SKIP_SPACE)
		else if (*ptr == ' ')
			Print_Space();
	#endif
		else
			Print_DrawChar(*ptr);

		ptr++;
	}

	va_end(args);
}
