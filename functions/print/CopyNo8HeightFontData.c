// Function: CopyNo8HeightFontData
// Source: print.c

void CopyNo8HeightFontData(const u8* src, u16 dst, u8 height)
{
	for (u8 i = 0; i < PRINT_DATA.CharCount; ++i)
	{
		VDP_FillVRAM(0, dst, 0, 8);
		VDP_WriteVRAM(src, dst, 0, height);
		src += height;
		dst += 8;
	}
}
