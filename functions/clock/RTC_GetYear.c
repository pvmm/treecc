// Function: RTC_GetYear
// Header: clock.h
// Source: clock.c

// Function: RTC_GetYear
// Get current clock year counter (0-99)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock year
u8 RTC_GetYear();

//-----------------------------------------------------------------------------
// Get current clock year counter (0-99)
u8 RTC_GetYear()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_YEAR);
	ret += 10 * RTC_Read(RTC_REG_TIME_10YEAR);
	return ret;
}
