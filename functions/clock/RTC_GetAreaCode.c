// Function: RTC_GetAreaCode
// Header: clock.h
// Source: clock.c

// Function: RTC_GetAreaCode
// Get area code
//
// Return:
//    Current area code
inline u8 RTC_GetAreaCode() { RTC_SetMode(RTC_MODE_SCREEN); return RTC_Read(RTC_REG_SYS_AREA_CODE); }
