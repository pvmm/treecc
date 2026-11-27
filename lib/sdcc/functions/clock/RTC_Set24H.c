// Function: RTC_Set24H
// Header: clock.h
// Source: clock.c

// Function: RTC_Set24H
// Set 12/24 hours mode
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Parameters:
//   enable - TRUE for 24h mode and FALSE for 12h mode
inline void RTC_Set24H(bool enable) { RTC_SetMode(RTC_MODE_ALARM); RTC_Write(RTC_REG_ALARM_24H, enable ? 1 : 0); }
