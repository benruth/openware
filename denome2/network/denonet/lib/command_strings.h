#ifndef COMMAND_STRINGS_H
#define COMMAND_STRINGS_H

static const char* str_channelvolume_command[] =
{
    "FL", "FR", "C", "SW", "SW2", "SL", "SR", "SBL", "SBR", "SB", "FHL", "FHR", "FWL", "FWR", "TFL", "TFR", "TML", "TMR", "TRL", "TRR", "RHL", "RHR", "FDL", "FDR", "FDL", "FDR", "SDL", "SDR", "BDL", "BDR", "SHL", "SHR", "TS", "CH"
};

static const char* str_signalinput_command[] = {
    "PHONE", "CD", "DVD", "BD", "TV", "SAT/CBL", "MPLAY", "GAME", "TUNER", "HDRADIO", "IRADIO", "SERVER", "FAVORITES", "AUX1", "AUX2", "AUX3", "AUX4", "AUX5", "AUX6", "AUX7", "NET", "BT"
};

static const char* str_autostandby_command[] =
{
    "15M", "30M", "60M", "OFF"
};

static const char* str_menucontrols_commands[] =
{
    "MEN ON", "MEN OFF", "OPT", "INF", "CHL", "CUP", "CDN", "CLT", "CRT", "ENT", "RTN"
};

static const char* str_dynamicrange_commands[] =
{
    "AUTO", "LOW", "MID", "HI", "OFF"
};

static const char* str_dimmerstate_commands[] =
{
    "BRI", "DIM", "DAR", "OFF", "SEL"
};

static const char* str_statusreq_commands[] =
{
    "PW?", "MV?", "CV?", "MU?", "SI?", "STBY?", "ECO?", "SLP?", "MS?", "PSSWR ?", "PSTONE CTRL ?", "PSBAS ?", "PSTRE ?", "MNMEN?", "DIM ?", "PSDRC ?"
};

static const char* str_surroundMode_commands[] =
{
    "PURE DIRECT",
    "DSD PURE DIRECT",
    "STEREO",
    "AUTO",
    "DOLBY DIGITAL",
    "DOLBY PRO LOGIC",
    "DOLBY PLII MV",
    "DOLBY PLII MS",
    "DOLBY PLII GM",
    "DOLBY PL2X C",
    "DOLBY PL2X M",
    "DOLBY PL2X G",
    "DOLBY PL2Z H",
    "DOLBY AUDIO-DSUR",
    "DOLBY ATMOS",
    "DOLBY AUDIO-DD",
    "DOLBY D EX",
    "DOLBY D+PL2X C",
    "DOLBY D+PL2X M",
    "DOLBY D+PL2Z H",
    "DOLBY AUDIO-DD+DSUR",
    "DOLBY D+NEO:X C",
    "DOLBY D+NEO:X M",
    "DOLBY D+NEO:X G",
    "DOLBY AUDIO-DD+NEURAL:X",
    "DTS SURROUND",
    "DTS ES DSCRT6.1",
    "DTS ES MTRX6.1",
    "DTS+PL2X C",
    "DTS+PL2X M",
    "DTS+PL2Z H",
    "DTS+DSUR",
    "DTS96/24",
    "DTS96 ES MTRX",
    "DTS+NEO:6",
    "DTS+NEO:X C",
    "DTS+NEO:X M",
    "DTS+NEO:X G",
    "DTS+NEURAL:X",
    "DTS+VIRTUAL:X",
    "DTS ES MTRX+NEURAL:X",
    "DTS ES DSCRT+NEURAL:X",
    "MULTI CH IN",
    "M CH IN+DOLBY EX",
    "M CH IN+PL2X C",
    "M CH IN+PL2X M",
    "M CH IN+PL2Z H",
    "M CH IN+DSUR",
    "MULTI CH IN 7.1",
    "M CH IN+NEO:X C",
    "M CH IN+NEO:X M",
    "M CH IN+NEO:X G",
    "M CH IN+NEURAL:X",
    "M CH IN+VIRTUAL:X",
    "DOLBY AUDIO-DD+",
    "DOLBY D+ +EX",
    "DOLBY D+ +PL2X C",
    "DOLBY D+ +PL2X M",
    "DOLBY D+ +PL2Z H",
    "DOLBY AUDIO-DD+ +DSUR",
    "DOLBY D+ +NEO:X C",
    "DOLBY D+ +NEO:X M",
    "DOLBY D+ +NEO:X G",
    "DOLBY AUDIO-DD+ +NERUAL:X",
    "DOLBY AUDIO-TRUEHD",
    "DOLBY HD+EX",
    "DOLBY HD+PL2X C",
    "DOLBY HD+PL2X M",
    "DOLBY HD+PL2Z H",
    "DOLBY AUDIO-TRUEHD+DSUR",
    "DOLBY HD+NEO:X C",
    "DOLBY HD+NEO:X M",
    "DOLBY HD+NEO:X G",
    "DOLBY AUDIO-TRUEHD+NEURAL:X",
    "DTS HD",
    "DTS HD MSTR",
    "DTS HD+PL2X C",
    "DTS HD+PL2X M",
    "DTS HD+PL2Z H",
    "DTS HD+DSUR",
    "DTS HD+NEO:6",
    "DTS HD+NEO:X C",
    "DTS HD+NEO:X M",
    "DTS HD+NEO:X G",
    "DTS HD+NEURAL:X",
    "DTS HD+VIRTUAL:X",
    "DTS:X",
    "DTS:X MSTR",
    "DTS:X+VIRTUAL:X",
    "DTS EXPRESS",
    "DTS ES 8CH DSCRT",
    "MPEG2 AAC",
    "AAC+DOLBY EX",
    "AAC+PL2X C",
    "AAC+PL2X M",
    "AAC+PL2Z H",
    "AAC+DSUR",
    "AAC+NEO:X C",
    "AAC+NEO:X M",
    "AAC+NEO:X G",
    "AAC+NEURAL:X",
    "AAC+VIRTUAL:X",
    "PL DSX",
    "PL2 C DSX",
    "PL2 M DSX",
    "PL2 G DSX",
    "PL2X C DSX",
    "PL2X M DSX",
    "PL2X G DSX",
    "AUDYSSEY DSX",
    "DTS NEO:6 C",
    "DTS NEO:6 M",
    "DTS NEO:X C",
    "DTS NEO:X M",
    "DTS NEO:X G",
    "NEURAL:X",
    "VIRTUAL:X",
    "DOLBY D+NEURAL:X",
    "DOLBY D+",
    "DOLBY D+ +PLZ H",
    "DOLBY D+ +NEURAL:X",
    "DOLBY HD",
    "DOLBY HD+NEURAL:X",
    "NEO:6 C DSX",
    "NEO:6 M DSX",
    "AURO3D",
    "AURO2DSURR",
    "MCH STEREO",
    "WIDE SCREEN",
    "SUPER STADIUM",
    "ROCK ARENA",
    "JAZZ CLUB",
    "CLASSIC CONCERT",
    "MONO MOVIE",
    "MATRIX",
    "VIDEO GAME",
    "VIRTUAL"
};

#endif // COMMAND_STRINGS_H
