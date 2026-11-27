// Function: RTC_SaveData
// Header: clock.h
// Source: clock.c

// Function: RTC_SaveData
// Save 6 bytes data to CMOS's block 3
// Need RTC_USE_SAVEDATA define to be set in MSXgl configuration
//
// Parameters:
//   data - 6 bytes buffer to be saved
void RTC_SaveData(u8* data);

//-----------------------------------------------------------------------------
// Save 6 bytes data to CMOS's block 3
void RTC_SaveData(u8* data)
{
	RTC_SetMode(RTC_MODE_BLOCK_3);
	RTC_Write(0, RTC_DATA_SAVE);
	RTC_WriteRaw(1, data, 6);
}
