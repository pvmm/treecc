// Function: RTC_IsSettingOK
// Header: clock.h
// Source: clock.c

// Function: RTC_IsSettingOK
// Check if setting data is valid
//
// Return:
//   FALSE if setting is invalid (system information is not used or initialized)
inline bool RTC_IsSettingOK() { RTC_SetMode(RTC_MODE_SCREEN); return RTC_Read(RTC_REG_SYS_INIT) == RTC_INIT_DONE; }
