// Function: RTC_GetHour
// Header: clock.h
// Source: clock.c

// Function: RTC_GetHour
// Get current clock hour counter (0-23)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock hour
u8 RTC_GetHour();

//-----------------------------------------------------------------------------
// Get current clock hour counter (0-23)
u8 RTC_GetHour()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_HOUR);
	ret += 10 * RTC_Read(RTC_REG_TIME_10HOUR);
	return ret;
}
