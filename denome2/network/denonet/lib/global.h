

#ifndef GLOBAL_H
#define GLOBAL_H


#define DENON_PORT 23
#define PING_TIME 5000
#define PING_TIME_LONG 30000
#define RECONNECT_TO 5000
#define WAITING_TO 500


#define DELAYED true
#define NOT_DELAYED false

typedef enum {
    ON,
    STANDBY,
    DISCONNECTED
} DenonState;

typedef enum {
    ST_MOVIE,
    ST_MUSIC,
    ST_GAME,
    ST_DIRECT,

    STYPE_NUM
} SoundType;

typedef enum
{
    MSPURE_DIRECT,
    MS_DSD_PURE_DIRECT,
    MS_STEREO,
    MS_AUTO,
    MS_DOLBY_DIGITAL,
    MS_DOLBY_PRO_LOGIC,
    MS_DOLBY_PLII_MV,
    MS_DOLBY_PLII_MS,
    MS_DOLBY_PLII_GM,
    MS_DOLBY_PL2X_C,
    MS_DOLBY_PL2X_M,
    MS_DOLBY_PL2X_G,
    MS_DOLBY_PL2Z_H,
    MS_DOLBY_AUDIO_DSUR,
    MS_DOLBY_ATMOS,
    MS_DOLBY_AUDIO_DD,
    MS_DOLBY_D_EX,
    MS_DOLBY_DPPL2X_C,
    MS_DOLBY_DPPL2X_M,
    MS_DOLBY_DPPL2Z_H,
    MS_DOLBY_AUDIO_DDPDSUR,
    MS_DOLBY_DPNEO_X_C,
    MS_DOLBY_DPNEO_X_M,
    MS_DOLBY_DPNEO_X_G,
    MS_DOLBY_AUDIO_DDPNEURAL_X,
    MS_DTS_SURROUND,
    MS_DTS_ES_DSCRT6_d_1,
    MS_DTS_ES_MTRX6_d_1,
    MS_DTSPPL2X_C,
    MS_DTSPPL2X_M,
    MS_DTSPPL2Z_H,
    MS_DTSPDSUR,
    MS_DTS96_24,
    MS_DTS96_ES_MTRX,
    MS_DTSPNEO_6,
    MS_DTSPNEO_X_C,
    MS_DTSPNEO_X_M,
    MS_DTSPNEO_X_G,
    MS_DTSPNEURAL_X,
    MS_DTSPVIRTUAL_X,
    MS_DTS_ES_MTRXPNEURAL_X,
    MS_DTS_ES_DSCRTPNEURAL_X,
    MS_MULTI_CH_IN,
    MS_M_CH_INPDOLBY_EX,
    MS_M_CH_INPPL2X_C,
    MS_M_CH_INPPL2X_M,
    MS_M_CH_INPPL2Z_H,
    MS_M_CH_INPDSUR,
    MS_MULTI_CH_IN_7_d_1,
    MS_M_CH_INPNEO_X_C,
    MS_M_CH_INPNEO_X_M,
    MS_M_CH_INPNEO_X_G,
    MS_M_CH_INPNEURAL_X,
    MS_M_CH_INPVIRTUAL_X,
    MS_DOLBY_AUDIO_DDP,
    MS_DOLBY_DP_PEX,
    MS_DOLBY_DP_PPL2X_C,
    MS_DOLBY_DP_PPL2X_M,
    MS_DOLBY_DP_PPL2Z_H,
    MS_DOLBY_AUDIO_DDP_PDSUR,
    MS_DOLBY_DP_PNEO_X_C,
    MS_DOLBY_DP_PNEO_X_M,
    MS_DOLBY_DP_PNEO_X_G,
    MS_DOLBY_AUDIO_DDP_PNERUAL_X,
    MS_DOLBY_AUDIO_TRUEHD,
    MS_DOLBY_HDPEX,
    MS_DOLBY_HDPPL2X_C,
    MS_DOLBY_HDPPL2X_M,
    MS_DOLBY_HDPPL2Z_H,
    MS_DOLBY_AUDIO_TRUEHDPDSUR,
    MS_DOLBY_HDPNEO_X_C,
    MS_DOLBY_HDPNEO_X_M,
    MS_DOLBY_HDPNEO_X_G,
    MS_DOLBY_AUDIO_TRUEHDPNEURAL_X,
    MS_DTS_HD,
    MS_DTS_HD_MSTR,
    MS_DTS_HDPPL2X_C,
    MS_DTS_HDPPL2X_M,
    MS_DTS_HDPPL2Z_H,
    MS_DTS_HDPDSUR,
    MS_DTS_HDPNEO_6,
    MS_DTS_HDPNEO_X_C,
    MS_DTS_HDPNEO_X_M,
    MS_DTS_HDPNEO_X_G,
    MS_DTS_HDPNEURAL_X,
    MS_DTS_HDPVIRTUAL_X,
    MS_DTS_X,
    MS_DTS_X_MSTR,
    MS_DTS_XPVIRTUAL_X,
    MS_DTS_EXPRESS,
    MS_DTS_ES_8CH_DSCRT,
    MS_MPEG2_AAC,
    MS_AACPDOLBY_EX,
    MS_AACPPL2X_C,
    MS_AACPPL2X_M,
    MS_AACPPL2Z_H,
    MS_AACPDSUR,
    MS_AACPNEO_X_C,
    MS_AACPNEO_X_M,
    MS_AACPNEO_X_G,
    MS_AACPNEURAL_X,
    MS_AACPVIRTUAL_X,
    MS_PL_DSX,
    MS_PL2_C_DSX,
    MS_PL2_M_DSX,
    MS_PL2_G_DSX,
    MS_PL2X_C_DSX,
    MS_PL2X_M_DSX,
    MS_PL2X_G_DSX,
    MS_AUDYSSEY_DSX,
    MS_DTS_NEO_6_C,
    MS_DTS_NEO_6_M,
    MS_DTS_NEO_X_C,
    MS_DTS_NEO_X_M,
    MS_DTS_NEO_X_G,
    MS_NEURAL_X,
    MS_VIRTUAL_X,
    MS_DOLBY_DPNEURAL_X,
    MS_DOLBY_DP,
    MS_DOLBY_DP_PPLZ_H,
    MS_DOLBY_DP_PNEURAL_X,
    MS_DOLBY_HD,
    MS_DOLBY_HDPNEURAL_X,
    MS_NEO_6_C_DSX,
    MS_NEO_6_M_DSX,
    MS_AURO3D,
    MS_AURO2DSURR,
    MS_MCH_STEREO,
    MS_WIDE_SCREEN,
    MS_SUPER_STADIUM,
    MS_ROCK_ARENA,
    MS_JAZZ_CLUB,
    MS_CLASSIC_CONCERT,
    MS_MONO_MOVIE,
    MS_MATRIX,
    MS_VIDEO_GAME,
    MS_VIRTUAL,
    MS_Nums

} SurroundMode;


static const char* str_soundtypes[] =
{
    "MOVIE",
    "MUSIC",
    "GAME",
    "DIRECT"
};


typedef enum  {
    CH_FRONT_LEFT,
    CH_FRONT_RIGHT,
    CH_CENTER,
    CH_SUBWOOFER,
    CH_SUBWOOFER2,
    CH_SUR_LEFT,
    CH_SUR_RIGHT,
    CH_SUR_B_L,
    CH_SUR_B_R,
    CH_SUR_B,
    CH_FRONT_H_L,
    CH_FRONT_H_R,
    CH_FRONT_WIDE_L,
    CH_FRONT_WIDE_R,
    CH_TOP_FRONT_L,
    CH_TOP_FRONT_R,
    CH_TOP_MID_L,
    CH_TOP_MID_R,
    CH_TOP_REAR_L,
    CH_TOP_REAR_R,
    CH_REAR_H_L,
    CH_REAR_H_R,
    CH_FRONT_DOLBY_L,
    CH_FRONT_DOLBY_R,
    CH_SUR_DOLBY_L,
    CH_SUR_DOLBY_R,
    CH_BACK_DOLBY_L,
    CH_BACK_DOLBY_R,
    CH_SUR_H_L,
    CH_SUR_H_R,
    CH_TOP_SUR,
    CH_CENTER_H,

    CHANNEL_VOL_NUM

} ChannelVolume;


static const char* str_channelvolume[] =
{
    "Front Left",
    "Front Right",
    "Center",
    "Subwoofer 1",
    "Subwoofer 2",
    "Surround Left",
    "Surround Right",
    "Surround Back Left",
    "Surround Back Right",
    "Surround Back",
    "Front Height Left",
    "Front Height Right",
    "Front Wide Left",
    "Front Wide Right",
    "Top Front Left",
    "Top Front Right",
    "Top Middle Left",
    "Top Middle Right",
    "Top Rear Left",
    "Top Rear Right",
    "Rear Height Left",
    "Rear Heigt Right",
    "Dolby Front Left",
    "Dolby Front Right",
    "Dolby Surround Left",
    "Dolby Surround Right",
    "Dolby Back Left",
    "Dolby Back Right",
    "Surround Height Left",
    "Surround Heigt Right",
    "Surround Top",
    "Center Height"
};



typedef enum {
    ECO_ON,
    ECO_AUTO,
    ECO_OFF
} EcoStates;

static const char* str_ecostates[] =
{
    "Ein", "Auto", "Aus"
};

typedef enum
{
    SI_PHONE,
    SI_CD,
    SI_DVD,
    SI_BD,
    SI_TV,
    SI_SAT,
    SI_MPLAY,
    SI_GAME,
    SI_TUNER,
    SI_HDRADIO,
    SI_IRADIO,
    SI_SERVER,
    SI_FAVOURITE,
    SI_AUX1,
    SI_AUX2,
    SI_AUX3,
    SI_AUX4,
    SI_AUX5,
    SI_AUX6,
    SI_AUX7,
    SI_NET,
    SI_BT,

    SI_NUMS
} SignalInputs;

static const char* str_signalinputs[] =
{
    "Phone", "CD", "DVD", "BluRay", "TV Audio", "Sat/CBL", "Media Player", "Game", "Tuner",  "HD Radio", "Internet Radio", "Server", "Favouriten", "AUX 1", "AUX 2", "AUX 3", "AUX 4", "AUX 5", "AUX 6", "AUX 7", "Network", "Bluetooth"
};


typedef enum
{
    ASTBY_15M,
    ASTBY_30M,
    ASTBY_60M,
    ASTBY_OFF,

    ASTBY_NUM
} AutoStandby;



typedef enum
{


    MN_ON,
    MN_OFF,
    MN_OPTION,
    MN_INFO,
    MN_CHANNEL_ADJ,

    MN_UP,
    MN_DOWN,
    MN_LEFT,
    MN_RIGHT,
    MN_ENTER,
    MN_RETURN,

    MN_NUMS
} MenuControls;


typedef enum
{
    DRC_AUTO,
    DRC_LOW,
    DRC_MID,
    DRC_HI,
    DRC_OFF,

    DRC_NUMS
} DRC_Modes;    //dynamic range compression


typedef enum
{
    DIM_BRIGHT,
    DIM_DIM,
    DIM_DARK,
    DIM_OFF,
    DIM_SEL,    //Toggle next State; send only

    DIM_NUMS
} DimmerState;

static const char* str_dimmerstate[] =
{
    "Bright", "Dim", "Dark", "Off", "Toggle"
};

typedef enum
{
    SR_POWER,
    SR_VOLUME,
    SR_CHANNEL_VOL,
    SR_MUTE,
    SR_SIGNAL_INPUT,
    SR_AUTO_STANDBY,
    SR_ECO,
    SR_SLEEP,
    SR_SURROUND_MODE,
    SR_SUBWOOFER_ACTIVE,
    SR_TONE_CTRL,
    SR_TONE_BASS,
    SR_TONE_TREBLE,
    SR_MENU,
    SR_DIMMER,
    SR_DRC
} StatusRequests;


#endif // GLOBAL_H
