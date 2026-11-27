// Function: RTC_GetDataType
// Header: clock.h
// Source: clock.c

// Function: RTC_GetDataType
// Get current data type
//
// Return:
//    Current data type (see RTC_DATA_ defines)
inline u8 RTC_GetDataType() { RTC_SetMode(RTC_MODE_DATA); return RTC_Read(RTC_REG_DATATYPE); }
