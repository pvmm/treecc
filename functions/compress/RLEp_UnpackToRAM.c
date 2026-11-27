// Function: RLEp_UnpackToRAM
// Header: compress.h
// Source: compress.c

// Function: RLEp_UnpackToRAM
// Unpack RLEp compressed data to memory
//
// Parameters:
//   src - Source data
//   dst - Destination data in RAM
//   size - Size of the data to unpack (only if COMPRESS_USE_RLEP_FIXSIZE is defined)
//
// Return:
//   Unpacked data size
u16 RLEp_UnpackToRAM(const u8* src, u8* dst RLEP_FIXSIZE_PARAM);

//-----------------------------------------------------------------------------
/// Unpack RLEp compressed data to memory
u16 RLEp_UnpackToRAM(const u8* src, u8* dst RLEP_FIXSIZE_PARAM)
{
	RLEP_DEFAULT_SET

	u16 start = (u16)dst;

	RLEP_FIXSIZE_WHILE
	{
		// Unpack chunk header
		u8 type = *src >> 6;
		u8 count = (*src & 0x3F) + 1;

		src++;
		if (type == 0) // Chunk of zeros
		{
			Mem_Set(RLEP_DEFAULT_GET, dst, count);
		}
		else if (type == 1) // Chunk of same byte
		{
			Mem_Set(*src, dst, count);
			src++;
		}
		else if (type == 2) // Chunk of same 2 bytes
		{
			count <<= 1;
			for (u8 i = 0; i < count; ++i)
				dst[i] = src[i & 0x1];
			src += 2;
		}
		else // type == 3 // Chunk of uncompressed data
		{
			Mem_Copy(src, dst, count);
			src += count;
		}
		dst += count;
	}
	return ((u16)dst - start);
}
