// Function: RTC_GetSecond
// Header: clock.h
// Source: clock.c

// Function: RTC_GetSecond
// Get current clock second counter (0-59)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock second
u8 RTC_GetSecond();

//-----------------------------------------------------------------------------
// Get current clock second counter (0-59)
u8 RTC_GetSecond()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_SEC);
	ret += 10 * RTC_Read(RTC_REG_TIME_10SEC);
	return ret;
}
