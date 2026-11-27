// Function: RTC_GetYear4
// Header: clock.h
// Source: clock.c

// Function: RTC_GetYear4
// Get current clock 4-digits year counter (1980-2079)
// Need RTC_USE_CLOCK and RTC_USE_CLOCK_EXTRA define to be set in MSXgl configuration
//
// Return:
//   4-digits year counter
u16 RTC_GetYear4();

//-----------------------------------------------------------------------------
// Get current clock 4-number year counter (1980-2079)
u16 RTC_GetYear4()
{
	return 1980 + RTC_GetYear();
}
