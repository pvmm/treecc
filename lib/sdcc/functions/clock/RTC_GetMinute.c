// Function: RTC_GetMinute
// Header: clock.h
// Source: clock.c

// Function: RTC_GetMinute
// Get current clock minute counter (0-59)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock minute
u8 RTC_GetMinute();

//-----------------------------------------------------------------------------
// Get current clock minute counter (0-59)
u8 RTC_GetMinute()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_MIN);
	ret += 10 * RTC_Read(RTC_REG_TIME_10MIN);
	return ret;
}
