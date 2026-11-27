// Function: RTC_ReadRaw
// Header: clock.h
// Source: clock.c

// Function: RTC_ReadRaw
// Read data from CMOS in the current block
// Use RTC_SetMode to change current block
//
// Parameters:
//   reg - Register index to start reading
//   data - Data to read
//   num - Number of bytes to read (from 1 to 6)
void RTC_ReadRaw(u8 reg, u8* data, u8 num);

//-----------------------------------------------------------------------------
// Read data from CMOS
void RTC_ReadRaw(u8 reg, u8* data, u8 num)
{
	for (u8 i = 0; i < num; ++i)
	{
		*data = RTC_Read(reg++);
		*data |= RTC_Read(reg++) << 4;
		data++;
	}
}
