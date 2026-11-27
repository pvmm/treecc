// Function: RTC_SaveDataSigned
// Header: clock.h
// Source: clock.c

// Function: RTC_SaveDataSigned
// Save 6 bytes data to CMOS's block 3
// Need RTC_USE_SAVESIGNED define to be set in MSXgl configuration and AppSignature BuildTool option to TRUE
//
// Parameters:
//   data - 6 bytes buffer to be saved
void RTC_SaveDataSigned(u8* data);

//-----------------------------------------------------------------------------
// Save 6 bytes data to CMOS's block 3
void RTC_SaveDataSigned(u8* data)
{
	// Write application signature
	RTC_SetMode(RTC_MODE_BLOCK_1);
	const u8* sign = (const u8*)&g_AppSignature;
	// Signature 1st byte (company LSB)
 	RTC_Write(2, *sign      & 0xF ); // 4 bits
 	RTC_Write(3, *sign >> 4 & 0x7 ); // 3 bits
	sign++;
	// Signature 2nd byte (company MSB)
 	RTC_Write(4, *sign      & 0xF ); // 4 bits
 	RTC_Write(6, *sign >> 4 & 0x7 ); // 3 bits
	sign++;
	// Signature 4th byte (product LSB)
 	RTC_Write(5, *sign      & 0x3 ); // 2 bits
 	RTC_Write(7, *sign >> 2 & 0xF ); // 4 bits
 	RTC_Write(8, *sign >> 6 & 0x3 ); // 2 bits

	// Write data
	RTC_SetMode(RTC_MODE_BLOCK_3);
	RTC_Write(0, RTC_DATA_SIGNSAVE);
	RTC_WriteRaw(1, data, 6);
}
