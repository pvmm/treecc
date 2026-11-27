// Function: RTC_GetDay
// Header: clock.h
// Source: clock.c

// Function: RTC_GetDay
// Get current clock day counter (1-31)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock day
u8 RTC_GetDay();

//-----------------------------------------------------------------------------
// Get current clock day counter (1-31)
u8 RTC_GetDay()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_DAY);
	ret += 10 * RTC_Read(RTC_REG_TIME_10DAY);
	return ret;
}
