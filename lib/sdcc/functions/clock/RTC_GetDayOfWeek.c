// Function: RTC_GetDayOfWeek
// Header: clock.h
// Source: clock.c

// Function: RTC_GetDayOfWeek
// Get current clock day-of-week counter (0-6)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock day-of-week
u8 RTC_GetDayOfWeek();

//-----------------------------------------------------------------------------
// Get current clock day-of-week counter (0-6)
u8 RTC_GetDayOfWeek()
{
	RTC_SetMode(RTC_MODE_TIME);
	return RTC_Read(RTC_REG_TIME_WEEKDAY);
}
