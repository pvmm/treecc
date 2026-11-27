// Function: BIOS_PlayPSG
// Header: bios.h
// Source: bios.c

// Function: BIOS_PlayPSG
// Tests whether the PLAY statement is being executed as a background task. Wrapper for STRTMS routine.
//> STRTMS
//> Address  : #0099
//> Function : Tests whether the PLAY statement is being executed as a background
//>            task. If not, begins to execute the PLAY statement
//> Registers: All
//
// Parameters:
//   qcb - Pointer to 3 QCB structures containing the music data to play
//
// Return:
//   FALSE if the PLAY statement is already being executed as a background task.
bool BIOS_PlayPSG(const BIOS_QCB* qcb);
