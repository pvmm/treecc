// Function: RTC_LoadData
// Header: clock.h
// Source: clock.c

// Function: RTC_LoadData
// Load 6 bytes data from CMOS's block 3
// Need RTC_USE_SAVEDATA define to be set in MSXgl configuration
//
// Parameters:
//   data - 6 bytes buffer to load data
//
// Return:
//   FALSE if data loading failed
bool RTC_LoadData(u8* data);

//-----------------------------------------------------------------------------
// Load 6 bytes data from CMOS's block 3
bool RTC_LoadData(u8* data)
{
	RTC_SetMode(RTC_MODE_BLOCK_3);
	if (RTC_Read(0) != RTC_DATA_SAVE) // Check data type
		return FALSE;
	RTC_ReadRaw(1, data, 6);
	return TRUE;
}
