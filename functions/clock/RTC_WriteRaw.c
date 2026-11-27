// Function: RTC_WriteRaw
// Header: clock.h
// Source: clock.c

// Function: RTC_WriteRaw
// Write data in CMOS in the current block
// Use RTC_SetMode to change current block
//
// Parameters:
//   reg - Register index to start copy
//   data - Data to copy
//   num - Number of bytes to copy (from 1 to 6)
void RTC_WriteRaw(u8 reg, u8* data, u8 num);

//-----------------------------------------------------------------------------
// Write data in CMOS
void RTC_WriteRaw(u8 reg, u8* data, u8 num)
{
	for (u8 i = 0; i < num; ++i)
	{
		RTC_Write(reg++, *data & 0xF);
		RTC_Write(reg++, *data >> 4);
		data++;
	}
}
