// Function: RTC_LoadDataSigned
// Header: clock.h
// Source: clock.c

// Function: RTC_LoadDataSigned
// Load 6 bytes data from CMOS's block 3
// Need RTC_USE_SAVESIGNED define to be set in MSXgl configuration and AppSignature BuildTool option to TRUE
//
// Parameters:
//   data - 6 bytes buffer to load data
bool RTC_LoadDataSigned(u8* data);

//-----------------------------------------------------------------------------
// Load 6 bytes data from CMOS's block 3
bool RTC_LoadDataSigned(u8* data)
{
	// Check application signature
	RTC_SetMode(RTC_MODE_BLOCK_1);
	const u8* sign = (const u8*)&g_AppSignature;
	// Signature 1st byte (company LSB)
	u8 a = *sign & 0x7F;
	u8 b = RTC_Read(2) + (RTC_Read(3) << 4);
	if (a != b)
		return FALSE;
	// Signature 2nd byte (company MSB)
	sign++;
	a = *sign & 0x7F;
	b = RTC_Read(4) + (RTC_Read(6) << 4);
	if (a != b)
		return FALSE;
	// Signature 4th byte (product LSB)
	sign++;
	a = *sign;
	b = RTC_Read(5) + (RTC_Read(7) << 2) + (RTC_Read(8) << 6);
	if (a != b)
		return FALSE;

	// Load data
	RTC_SetMode(RTC_MODE_BLOCK_3);
	if (RTC_Read(0) != RTC_DATA_SIGNSAVE)
		return FALSE;
	RTC_ReadRaw(1, data, 6);
	return TRUE;
}
