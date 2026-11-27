// Function: RTC_SetMode
// Header: clock.h
// Source: clock.c

// Function: RTC_SetMode
// Set clock mode
//
// Parameters:
//   mode - The clock mode flags
inline void RTC_SetMode(u8 mode) { io_RTC_Reg = RTC_REG_MODE; io_RTC_Data = mode | (RTC_MODE_ALARM_OFF + RTC_MODE_TIMER_ON); }
