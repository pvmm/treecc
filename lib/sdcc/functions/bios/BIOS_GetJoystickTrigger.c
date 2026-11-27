// Function: BIOS_GetJoystickTrigger
// Header: bios.h
// Source: bios.c

// Function: BIOS_GetJoystickTrigger
// Returns the state of the mouse, joystick or keyboard space bar buttons.
// Wrapper for GTTRIG routine.
//
// Parameters:
//   trigger - Trigger button to test:
//> 0 = space bar
//> 1 = port 1, button A
//> 2 = port 2, button A
//> 3 = port 1, button B
//> 4 = port 2, button B
//
// Return:
//   FALSE if trigger button is not pressed
inline bool BIOS_GetJoystickTrigger(u8 trigger) { return CallAToA(R_GTTRIG, trigger); }
