// Function: RTC_SetAreaCode
// Header: clock.h
// Source: clock.c

// Function: RTC_SetAreaCode
// Set area code
//
// Parameers:
//   code -  New area code to set
inline void RTC_SetAreaCode(u8 code) { RTC_SetMode(RTC_MODE_SCREEN); RTC_Write(RTC_REG_SYS_AREA_CODE, code); }
