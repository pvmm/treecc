// Function: BIOS_GetJoystickDirection
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetJoystickDirection
// Returns the state of the joystick or the cursor keys.
// Wrapper for GTSTCK routine.
//
// Parameters:
//   port - Joystick number to be tested:
//> 0 = Cursor keys
//> 1 = Joystick on port 1
//> 2 = Joystick on port 2
//
// Return:
//   Direction of the joystick or function keys (0-8):
//>  8  1  2
//>   \ | /
//> 7 - 0 - 3
//>   / |'\'
//>  6  5  4
inline u8 BIOS_GetJoystickDirection(u8 port) { return CallAToA(R_GTSTCK, port); }
