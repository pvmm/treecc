// Function: RTC_IsPM
// Header: clock.h
// Source: clock.c

// Function: RTC_IsPM
// Check if current hour is PM or AM
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   FALSE if current hour is AM
inline bool RTC_IsPM() { return RTC_GetHour() >= 12; }
