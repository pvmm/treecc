// Function: RTC_Write
// Header: clock.h
// Source: clock.c

// Function: RTC_Write
// Write a RTC register value
//
// Parameters:
//   reg - RTC register number
//   value - The value to write in the register
inline void RTC_Write(u8 reg, u8 value) { io_RTC_Reg = reg; io_RTC_Data = value; }
