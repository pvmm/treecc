// Function: MSXMusic_GetChannelType
// Header: msx-music.h
// Source: msx-music.c

// Function: MSXMusic_GetChannelType
// Get the type of a given channel
//
// Parameters:
//   chan - Channel index (0 to 13)
//
// Return:
//   Channel type
inline u8 MSXMusic_GetChannelType(u8 chan) { return (chan < 9) ? CHANNEL_TYPE_FM_INST : CHANNEL_TYPE_FM_RHYTHM; }
