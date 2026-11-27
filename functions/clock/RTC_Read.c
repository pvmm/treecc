// Function: RTC_Read
// Header: clock.h
// Source: clock.c

// Function: RTC_Read
// Read a RTC register value
//
// Parameters:
//   reg - RTC register number
//
// Return:
//   Register value (4 bits)
inline u8 RTC_Read(u8 reg) { io_RTC_Reg = reg; return 0xF & io_RTC_Data; }
