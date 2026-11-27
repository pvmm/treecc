// Function: RTC_GetMonth
// Header: clock.h
// Source: clock.c

// Function: RTC_GetMonth
// Get current clock month counter (1-12)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock month
u8 RTC_GetMonth();

//-----------------------------------------------------------------------------
// Get current clock month counter (1-12)
u8 RTC_GetMonth()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_MONTH);
	ret += 10 * RTC_Read(RTC_REG_TIME_10MONTH);
	return ret;
}
