#include "retro_common.h"
#include "retro_input.h"
#include "burnint.h"
#include <string.h>

struct RomBiosInfo neogeo_bioses[] = {
	{"sp-s3.sp1",         0x91b64be3, 0x00, "MVS Asia/Europe ver. 6 (1 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-s2.sp1",         0x9036d879, 0x01, "MVS Asia/Europe ver. 5 (1 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-s.sp1",          0xc7f2fa45, 0x02, "MVS Asia/Europe ver. 3 (4 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-45.sp1",         0x03cc9f6a, 0x0b, "NEO-MVH MV1C (Asia)"            , NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-u2.sp1",         0xe72943de, 0x03, "MVS USA ver. 5 (2 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp1-u2",            0x62f021f4, 0x04, "MVS USA ver. 5 (4 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp-e.sp1",          0x2723a5b5, 0x05, "MVS USA ver. 5 (6 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp1-u4.bin",        0x1179a30f, 0x06, "MVS USA (U4)"                   , NEOGEO_MVS | NEOGEO_USA, 0 }, 
	{"sp1-u3.bin",        0x2025b7a2, 0x07, "MVS USA (U3)"                   , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"vs-bios.rom",       0xf0e8f27d, 0x08, "MVS Japan ver. 6 (? slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp-j2.sp1",         0xacede59C, 0x09, "MVS Japan ver. 5 (? slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp1.jipan.1024",    0x9fb0abe4, 0x0a, "MVS Japan ver. 3 (4 slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp-j3.sp1",         0x486cb450, 0x0c, "NEO-MVH MV1C (Japan)"           , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"japan-j3.bin",      0xdff6d41f, 0x0d, "MVS Japan (J3)"                 , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp1-j3.bin",        0xfbc6d469, 0x0e, "MVS Japan (J3, alt)"            , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"neo-epo.bin",       0xd27a71f1, 0x10, "AES Asia"                       , NEOGEO_AES | NEOGEO_EUR, 0 },
	{"neo-po.bin",        0x16d0c132, 0x0f, "AES Japan"                      , NEOGEO_AES | NEOGEO_JAP, 0 },
	{"uni-bios_4_0.rom",  0xa7aab458, 0x13, "Universe BIOS ver. 4.0"         , NEOGEO_UNI, 0 },
	{"uni-bios_3_3.rom",  0x24858466, 0x14, "Universe BIOS ver. 3.3"         , NEOGEO_UNI, 0 },
	{"uni-bios_3_2.rom",  0xa4e8b9b3, 0x15, "Universe BIOS ver. 3.2"         , NEOGEO_UNI, 0 },
	{"uni-bios_3_1.rom",  0x0c58093f, 0x16, "Universe BIOS ver. 3.1"         , NEOGEO_UNI, 0 },
	{"uni-bios_3_0.rom",  0xa97c89a9, 0x17, "Universe BIOS ver. 3.0"         , NEOGEO_UNI, 0 },
	{"uni-bios_2_3.rom",  0x27664eb5, 0x18, "Universe BIOS ver. 2.3"         , NEOGEO_UNI, 0 },
	{"uni-bios_2_3o.rom", 0x601720ae, 0x19, "Universe BIOS ver. 2.3 (alt)"   , NEOGEO_UNI, 0 },
	{"uni-bios_2_2.rom",  0x2d50996a, 0x1a, "Universe BIOS ver. 2.2"         , NEOGEO_UNI, 0 },
	{"uni-bios_2_1.rom",  0x8dabf76b, 0x1b, "Universe BIOS ver. 2.1"         , NEOGEO_UNI, 0 },
	{"uni-bios_2_0.rom",  0x0c12c2ad, 0x1c, "Universe BIOS ver. 2.0"         , NEOGEO_UNI, 0 },
	{"uni-bios_1_3.rom",  0xb24b44a0, 0x1d, "Universe BIOS ver. 1.3"         , NEOGEO_UNI, 0 },
	{"uni-bios_1_2.rom",  0x4fa698e9, 0x1e, "Universe BIOS ver. 1.2"         , NEOGEO_UNI, 0 },
	{"uni-bios_1_2o.rom", 0xe19d3ce9, 0x1f, "Universe BIOS ver. 1.2 (alt)"   , NEOGEO_UNI, 0 },
	{"uni-bios_1_1.rom",  0x5dda0d84, 0x20, "Universe BIOS ver. 1.1"         , NEOGEO_UNI, 0 },
	{"uni-bios_1_0.rom",  0x0ce453a0, 0x21, "Universe BIOS ver. 1.0"         , NEOGEO_UNI, 0 },
	{NULL, 0, 0, NULL, 0 }
};

std::vector<dipswitch_core_option> dipswitch_core_options;
struct GameInp *pgi_reset;
struct GameInp *pgi_diag;
struct GameInp *pgi_debug_dip_1;
struct GameInp *pgi_debug_dip_2;
bool bIsNeogeoCartGame = false;
bool allow_neogeo_mode = true;
bool neogeo_use_specific_default_bios = false;
bool bAllowDepth32 = false;
bool bPatchedRomsetsEnabled = true;
bool bLibretroSupportsAudioBuffStatus = false;
bool bLowPassFilterEnabled = false;
UINT32 nVerticalMode = 0;
UINT32 nFrameskip = 1;
INT32 g_audio_samplerate = 48000;
UINT32 nMemcardMode = 0;
UINT32 nLightgunCrosshairEmulation = 0;
UINT8 *diag_input;
uint32_t g_opt_neo_geo_mode = 0;

#ifdef USE_CYCLONE
// 0 - c68k, 1 - m68k
// we don't use cyclone by default because it breaks savestates cross-platform compatibility (including netplay)
int nSekCpuCore = 1;
static bool bCycloneEnabled = false;
#endif

static UINT8 diag_input_start[] =       {RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_start_a_b[] =   {RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_A, RETRO_DEVICE_ID_JOYPAD_B, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_start_l_r[] =   {RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_L, RETRO_DEVICE_ID_JOYPAD_R, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select[] =      {RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select_a_b[] =  {RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_A, RETRO_DEVICE_ID_JOYPAD_B, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select_l_r[] =  {RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_L, RETRO_DEVICE_ID_JOYPAD_R, RETRO_DEVICE_ID_JOYPAD_EMPTY };

// Global core options
static const struct retro_core_option_v2_definition var_empty = { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL };
static const struct retro_core_option_v2_definition var_fbneo_allow_depth_32 = {
	"fbneo-allow-depth-32",
	"Use 32-bits color depth when available",
	NULL,
	"Change pixel format, some games require this to render properly, it could impact performances on some platforms",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_vertical_mode = {
	"fbneo-vertical-mode",
	"Vertical mode",
	NULL,
	"Rotate display for vertical screens",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ "alternate", NULL },
		{ "TATE", NULL },
		{ "TATE alternate", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_force_60hz = {
	"fbneo-force-60hz",
	"Force 60Hz",
	NULL,
	"Force 60Hz instead of original refresh rate",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_fixed_frameskip = {
	"fbneo-fixed-frameskip",
	"Fixed Frameskip",
	NULL,
	"When 'Frameskip' is set to 'Fixed', or if the frontend doesn't support the alternative 'Frameskip' mode, skip rendering at a fixed rate of X frames out of X+1",
	NULL,
	"frameskip",
	{
		{ "0", "No skipping" },
		{ "1", "Skip rendering of 1 frames out of 2" },
		{ "2", "Skip rendering of 2 frames out of 3" },
		{ "3", "Skip rendering of 3 frames out of 4" },
		{ "4", "Skip rendering of 4 frames out of 5" },
		{ "5", "Skip rendering of 5 frames out of 6" },
		{ NULL, NULL },
	},
	"0"
};
static const struct retro_core_option_v2_definition var_fbneo_frameskip_type = {
	"fbneo-frameskip-type",
	"Frameskip",
	NULL,
	"Skip frames to avoid audio buffer under-run (crackling). Improves performance at the expense of visual smoothness. 'Auto' skips frames when advised by the frontend. 'Manual' uses the 'Frameskip Threshold (%)' setting. 'Fixed' uses the 'Fixed Frameskip' setting.",
	NULL,
	"frameskip",
	{
		{ "disabled", NULL },
		{ "Fixed", NULL },
		{ "Auto", NULL },
		{ "Manual", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_frameskip_manual_threshold = {
	"fbneo-frameskip-manual-threshold",
	"Frameskip Threshold (%)",
	NULL,
	"When 'Frameskip' is set to 'Manual', specifies the audio buffer occupancy threshold (percentage) below which frames will be skipped. Higher values reduce the risk of crackling by causing frames to be dropped more frequently.",
	NULL,
	"frameskip",
	{
		{ "15", NULL },
		{ "18", NULL },
		{ "21", NULL },
		{ "24", NULL },
		{ "27", NULL },
		{ "30", NULL },
		{ "33", NULL },
		{ "36", NULL },
		{ "39", NULL },
		{ "42", NULL },
		{ "45", NULL },
		{ "48", NULL },
		{ "51", NULL },
		{ "54", NULL },
		{ "57", NULL },
		{ "60", NULL },
		{ NULL, NULL },
	},
	"33"
};
static const struct retro_core_option_v2_definition var_fbneo_cpu_speed_adjust = {
	"fbneo-cpu-speed-adjust",
	"CPU clock",
	NULL,
	"Change emulated cpu frequency for various systems, by increasing you can fix native slowdowns in some games, by decreasing you can help performance on low-end devices",
	NULL,
	NULL,
	{
		PERCENT_VALUES
	},
	"100%"
};
static const struct retro_core_option_v2_definition var_fbneo_diagnostic_input = {
	"fbneo-diagnostic-input",
	"Diagnostic Input",
	NULL,
	"Configure button combination to enter cabinet service menu",
	NULL,
	NULL,
	{
		{ "None", NULL },
		{ "Hold Start", NULL },
		{ "Start + A + B", NULL },
		{ "Hold Start + A + B", NULL },
		{ "Start + L + R", NULL },
		{ "Hold Start + L + R", NULL },
		{ "Hold Select", NULL },
		{ "Select + A + B", NULL },
		{ "Hold Select + A + B", NULL },
		{ "Select + L + R", NULL },
		{ "Hold Select + L + R", NULL },
		{ NULL, NULL },
	},
	"Hold Start"
};
static const struct retro_core_option_v2_definition var_fbneo_hiscores = {
	"fbneo-hiscores",
	"Hiscores",
	NULL,
	"Enable high scores support, you also need the file hiscore.dat in your system/fbneo/ folder",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_allow_patched_romsets = {
	"fbneo-allow-patched-romsets",
	"Allow patched romsets",
	NULL,
	"Allow romsets from your system/fbneo/patched/ folder to override your romsets, crcs will be ignored but sizes and names must still match, you need to close content for this setting to take effect",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_samplerate = {
	"fbneo-samplerate",
	"Samplerate",
	NULL,
	"Configure samplerate, it could impact performances, closing & starting game again is required",
	NULL,
	"audio",
	{
		{ "44100", NULL },
		{ "48000", NULL },
		{ NULL, NULL },
	},
	"48000"
};
static const struct retro_core_option_v2_definition var_fbneo_sample_interpolation = {
	"fbneo-sample-interpolation",
	"Sample Interpolation",
	NULL,
	"Configure sample interpolation, it could impact performances",
	NULL,
	"audio",
	{
		{ "disabled", NULL },
		{ "2-point 1st order", NULL },
		{ "4-point 3rd order", NULL },
		{ NULL, NULL },
	},
	"4-point 3rd order"
};
static const struct retro_core_option_v2_definition var_fbneo_fm_interpolation = {
	"fbneo-fm-interpolation",
	"FM Interpolation",
	NULL,
	"Configure FM interpolation, it could impact performances",
	NULL,
	"audio",
	{
		{ "disabled", NULL },
		{ "4-point 3rd order", NULL },
		{ NULL, NULL },
	},
	"4-point 3rd order"
};
static const struct retro_core_option_v2_definition var_fbneo_lowpass_filter = {
	"fbneo-lowpass-filter",
	"LowPass Filter",
	NULL,
	"Enable LowPass Filter",
	NULL,
	"audio",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_analog_speed = {
	"fbneo-analog-speed",
	"Analog Speed",
	NULL,
	"Mitigate analog controls speed, some games might require low values to be playable",
	NULL,
	NULL,
	{
		PERCENT_VALUES
	},
	"100%"
};
static const struct retro_core_option_v2_definition var_fbneo_lightgun_crosshair_emulation = {
	"fbneo-lightgun-crosshair-emulation",
	"Crosshair emulation",
	NULL,
	"Change emulated crosshair behavior",
	NULL,
	NULL,
	{
		{ "hide with lightgun device", NULL },
		{ "always hide", NULL },
		{ "always show", NULL },
		{ NULL, NULL },
	},
	"hide with lightgun device"
};
#ifdef USE_CYCLONE
static const struct retro_core_option_v2_definition var_fbneo_cyclone = {
	"fbneo-cyclone",
	"Enable cyclone",
	NULL,
	"Use at your own risk, it could improve performance on some emulated systems for low-end devices, but there are known side effects : savestates won't be compatible with normal interpreter, and some systems won't work",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
#endif

// Neo Geo core options
static const struct retro_core_option_v2_definition var_fbneo_neogeo_mode = {
	"fbneo-neogeo-mode",
	"Neo-Geo mode",
	NULL,
	"Load appropriate bios depending on your choice, under the condition such a bios is compatible with the running game",
	NULL,
	"neogeo",
	{
		{ "DIPSWITCH", "Use bios set in BIOS dipswitch" },
		{ "MVS_EUR", "MVS Europe/Asia (English)" },
		{ "MVS_USA", "MVS USA (English - Censored)" },
		{ "MVS_JAP", "MVS Japan (Japanese)" },
		{ "AES_EUR", "AES Europe/Asia (English)" },
		{ "AES_JAP", "AES Japan (Japanese)" },
		{ "UNIBIOS", "UNIBIOS" },
		{ NULL, NULL },
	},
	"DIPSWITCH"
};
static const struct retro_core_option_v2_definition var_fbneo_memcard_mode = {
	"fbneo-memcard-mode",
	"Memory card mode",
	NULL,
	"Change the behavior for the memory card",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "shared", NULL },
		{ "per-game", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_1 = {
	"fbneo-debug-dip-1-1",
	"Debug Dip 1_1",
	NULL,
	"Enable Debug Dip 1_1",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_2 = {
	"fbneo-debug-dip-1-2",
	"Debug Dip 1_2",
	NULL,
	"Enable Debug Dip 1_2",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_3 = {
	"fbneo-debug-dip-1-3",
	"Debug Dip 1_3",
	NULL,
	"Enable Debug Dip 1_3",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_4 = {
	"fbneo-debug-dip-1-4",
	"Debug Dip 1_4",
	NULL,
	"Enable Debug Dip 1_4",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_5 = {
	"fbneo-debug-dip-1-5",
	"Debug Dip 1_5",
	NULL,
	"Enable Debug Dip 1_5",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_6 = {
	"fbneo-debug-dip-1-6",
	"Debug Dip 1_6",
	NULL,
	"Enable Debug Dip 1_6",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_7 = {
	"fbneo-debug-dip-1-7",
	"Debug Dip 1_7",
	NULL,
	"Enable Debug Dip 1_7",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_1_8 = {
	"fbneo-debug-dip-1-8",
	"Debug Dip 1_8",
	NULL,
	"Enable Debug Dip 1_8",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_1 = {
	"fbneo-debug-dip-2-1",
	"Debug Dip 2_1",
	NULL,
	"Enable Debug Dip 2_1",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_2 = {
	"fbneo-debug-dip-2-2",
	"Debug Dip 2_2",
	NULL,
	"Enable Debug Dip 2_2",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_3 = {
	"fbneo-debug-dip-2-3",
	"Debug Dip 2_3",
	NULL,
	"Enable Debug Dip 2_3",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_4 = {
	"fbneo-debug-dip-2-4",
	"Debug Dip 2_4",
	NULL,
	"Enable Debug Dip 2_4",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_5 = {
	"fbneo-debug-dip-2-5",
	"Debug Dip 2_5",
	NULL,
	"Enable Debug Dip 2_5",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_6 = {
	"fbneo-debug-dip-2-6",
	"Debug Dip 2_6",
	NULL,
	"Enable Debug Dip 2_6",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_7 = {
	"fbneo-debug-dip-2-7",
	"Debug Dip 2_7",
	NULL,
	"Enable Debug Dip 2_7",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_dip_2_8 = {
	"fbneo-debug-dip-2-8",
	"Debug Dip 2_8",
	NULL,
	"Enable Debug Dip 2_8",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"disabled"
};


#ifdef FBNEO_DEBUG
static const struct retro_core_option_v2_definition var_fbneo_debug_layer_1 = {
	"fbneo-debug-layer-1",
	"Layer 1",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_layer_2 = {
	"fbneo-debug-layer-2",
	"Layer 2",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_layer_3 = {
	"fbneo-debug-layer-3",
	"Layer 3",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_layer_4 = {
	"fbneo-debug-layer-4",
	"Layer 4",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_1 = {
	"fbneo-debug-sprite-1",
	"Sprite 1",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_2 = {
	"fbneo-debug-sprite-2",
	"Sprite 2",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_3 = {
	"fbneo-debug-sprite-3",
	"Sprite 3",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_4 = {
	"fbneo-debug-sprite-4",
	"Sprite 4",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_5 = {
	"fbneo-debug-sprite-5",
	"Sprite 5",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_6 = {
	"fbneo-debug-sprite-6",
	"Sprite 6",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_7 = {
	"fbneo-debug-sprite-7",
	"Sprite 7",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
static const struct retro_core_option_v2_definition var_fbneo_debug_sprite_8 = {
	"fbneo-debug-sprite-8",
	"Sprite 8",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled", NULL },
		{ NULL, NULL },
	},
	"enabled"
};
#endif

// Replace the char c_find by the char c_replace in the destination c string
char* str_char_replace(char* destination, char c_find, char c_replace)
{
	for (unsigned str_idx = 0; str_idx < strlen(destination); str_idx++)
	{
		if (destination[str_idx] == c_find)
			destination[str_idx] = c_replace;
	}

	return destination;
}

void set_neogeo_bios_availability(char *szName, uint32_t crc, bool ignoreCrc)
{
	for (int i = 0; neogeo_bioses[i].filename != NULL; i++)
	{
		if ((strcmp(neogeo_bioses[i].filename, szName) == 0 && ignoreCrc) || neogeo_bioses[i].crc == crc)
		{
			neogeo_bioses[i].available = 1;
			return;
		}
	}
}

static RomBiosInfo* find_neogeo_bios(uint32_t categories)
{
	for (int i = 0; neogeo_bioses[i].filename != NULL; i++)
	{
		if (neogeo_bioses[i].categories == categories && neogeo_bioses[i].available == 1)
		{
			return &neogeo_bioses[i];
		}
	}

	return NULL;
}

void set_neo_system_bios()
{
	if (g_opt_neo_geo_mode == 0)
	{
		// Nothing to do in DIPSWITCH mode because the NeoSystem variable is changed by the DIP Switch core option
		log_cb(RETRO_LOG_INFO, "DIPSWITCH Neo Geo Mode selected => NeoSystem: 0x%02x.\n", NeoSystem);
	}
	else
	{
		RomBiosInfo *available_neogeo_bios = find_neogeo_bios(g_opt_neo_geo_mode);
		NeoSystem &= ~(UINT8)0x1f;
		if (available_neogeo_bios)
		{
			NeoSystem |= available_neogeo_bios->NeoSystem;
			log_cb(RETRO_LOG_INFO, "Found this bios for requested mode => NeoSystem: 0x%02x (%s [0x%08x] (%s)).\n", NeoSystem, available_neogeo_bios->filename, available_neogeo_bios->crc, available_neogeo_bios->friendly_name);
		}
		else
		{
			log_cb(RETRO_LOG_INFO, "No bios found for requested mode, falling back to DIPSWITCH => NeoSystem: 0x%02x.\n", NeoSystem);
		}
	}
}

void evaluate_neogeo_bios_mode(const char* drvname)
{
	if (!bIsNeogeoCartGame)
		return;

	bool is_bios_dipswitch_found = false;

	// search the BIOS dipswitch
	for (int dip_idx = 0; dip_idx < dipswitch_core_options.size(); dip_idx++)
	{
		if (dipswitch_core_options[dip_idx].friendly_name.compare("[Dipswitch] BIOS") == 0)
		{
			is_bios_dipswitch_found = true;
			if (dipswitch_core_options[dip_idx].values.size() > 0)
			{
				// if the default is different than 0, this means that a different Bios is needed
				if (dipswitch_core_options[dip_idx].default_bdi.nSetting != 0x00)
				{
					neogeo_use_specific_default_bios = true;
					break;
				}
			}
		}
	}

	// Games without the BIOS dipswitch don't handle alternative bioses very well
	if (!is_bios_dipswitch_found)
	{
		neogeo_use_specific_default_bios = true;
	}

	if (neogeo_use_specific_default_bios)
	{
		// disable the NeoGeo mode core option
		allow_neogeo_mode = false;

		// set the NeoGeo mode to DIPSWITCH to rely on the Default Bios Dipswitch
		g_opt_neo_geo_mode = 0;
	}
}

int get_supported_sound_channels(int type)
{
    // we performs 3 checks here in increasing order of accurancy to detect the current machine audio channels types
    unsigned curr_fm_channels = 0;
    unsigned curr_adpcm_channels = 0;
    unsigned curr_psg_channels = 0;
    
    // 1st check via DebugSnd_* globals from "burnint.h" (less reliable)

    if(DebugSnd_YM2151Initted) {
	log_cb(RETRO_LOG_INFO, "detected YM2151 sound chip, adding custom volume audio options\n");
	curr_fm_channels = 8;
	// test game: 1941, asuka
    }
    if(DebugSnd_YM2203Initted) {
	log_cb(RETRO_LOG_INFO, "detected YM2203 sound chip, adding custom volume audio options\n");
	curr_fm_channels = 3; // TODO: 4?
	// test game: lwing
    }
    if(DebugSnd_YM2608Initted) {
	log_cb(RETRO_LOG_INFO, "detected YM2608 sound chip, adding custom volume audio options\n");
	curr_fm_channels = 6;
	curr_adpcm_channels = 7;
	// test games: ??
    }
    if(DebugSnd_YM2610Initted) {
	log_cb(RETRO_LOG_INFO, "detected YM2610 sound chip, adding custom volume audio options\n");
	curr_fm_channels = 4;
	curr_adpcm_channels = 7;
	// test games: bbusters
    }
    if(DebugSnd_AY8910Initted) {
	log_cb(RETRO_LOG_INFO, "detected AY8910 sound chip, adding custom volume audio options\n");
	curr_psg_channels = 3;
	// test game: lwing
    }
    /* WIP:
    if(DebugSnd_MSM5205Initted) {
	log_cb(RETRO_LOG_INFO, "detected MSM5205 sound chip, adding custom volume audio options\n");
	curr_adpcm_channels = 12;
	// test games: ddragon
    }*/
    /*
    if(DebugSnd_MSM6295Initted) {
	log_cb(RETRO_LOG_INFO, "detected MSM6295 sound chip, adding custom volume audio options\n");
	curr_adpcm_channels = 4;
	// test games: toki?
    }*/
    /* WIP
    if(DebugSnd_K054539Initted) {
	log_cb(RETRO_LOG_INFO, "detected K054539 sound chip, adding custom volume audio options\n");
	curr_adpcm_channels = 8;
	// test games: xexex?
    }*/
    if(DebugSnd_SegaPCMInitted) {
		log_cb(RETRO_LOG_INFO, "detected Sega PCM sound chip, adding custom volume audio options\n");
		curr_adpcm_channels = 16;
		// test games: ??
    }
    if(DebugSnd_YMZ280BInitted) {
		log_cb(RETRO_LOG_INFO, "detected YMZ280B sound chip, adding custom volume audio options\n");
		curr_adpcm_channels = 8;
		// test games: esprade, cyvern
    }
    /* WIP
    if(DebugSnd_YM3812Initted) {
	log_cb(RETRO_LOG_INFO, "detected YM3812 sound chip, adding custom volume audio options\n");
	curr_fm_channels = 9;
	// test games: tndrcade  http://www.system16.com/hardware.php?id=966   http://www.system16.com/hardware.php?id=967
    }
    * */
    /* WIP
    if(DebugSnd_ES5506Initted) {
	log_cb(RETRO_LOG_INFO, "detected ES5505/6 sound chip, adding custom volume audio options\n");
	curr_adpcm_channels = 32;
	// test games: dynagear, arabianm, rayforce (taito f3)
    }*/
    // TODO: DebugSnd_DACInitted
    // test games: tankfrce
    /* WIP
    if(DebugSnd_ICS2115Initted) {
		log_cb(RETRO_LOG_INFO, "detected ICS2115 sound chip, adding custom volume audio options\n");
		curr_adpcm_channels = 32;
		// test games: ket?, .. (psm games)
	}*/
	if(DebugSnd_X1010Initted) {
		log_cb(RETRO_LOG_INFO, "detected Seta X1-010 sound chip, adding custom volume audio options\n");
		curr_adpcm_channels = 16;
		// test games: gundhara
	}

    // TODO: QSOUND?
    // TODO: PSG chips:
    // TODO:    TMS57002 http://www.system16.com/hardware.php?id=574
    // TODO:    GA20  http://www.system16.com/hardware.php?id=747&page=1#620
    // TODO: http://www.vgmpf.com/Wiki/index.php?title=YM3526
    // TODO: https://gist.github.com/bryc/e85315f758ff3eced19d2d4fdeef01c5
    // http://www.citylan.it/wiki/index.php/Konami_custom_chip
    
    // 2nd check via HARDWARE_* constants defined in "burn.h" (more reliable)
    INT32 hardware_code = BurnDrvGetHardwareCode();
    switch (hardware_code & HARDWARE_PUBLIC_MASK)
    {
	case HARDWARE_SNK_NEOGEO:
		log_cb(RETRO_LOG_INFO, "detected NEOGEO\n");
	    curr_fm_channels = 4;  // from YM2610
	    curr_adpcm_channels = 7;  // from YM2610
	    curr_psg_channels = 3;  // from AY8910
	    break;

	case HARDWARE_SEGA_SYSTEM16B:
	case HARDWARE_SEGA_SYSTEM16A:
	    log_cb(RETRO_LOG_INFO, "detected HARDWARE_SEGA_SYSTEM16\n");
	    curr_fm_channels = 8;  // from YM2151
	    //TODO: NEC uPD7751 ADPCM Decoder,  Nec uPD7759
	    break;
	    
	//TODO: case HARDWARE_SEGA_SYSTEM18: http://www.system16.com/hardware.php?id=702
	//TODO: case HARDWARE_SEGA_SYSTEM32: http://www.system16.com/hardware.php?id=709
	// 2 x YM3438 + Ricoh RF5c68
	
	case HARDWARE_CAPCOM_CPS1:
	case HARDWARE_CAPCOM_CPS1_GENERIC:
	case HARDWARE_CAPCOM_CPSCHANGER:
		log_cb(RETRO_LOG_INFO, "detected HARDWARE_CAPCOM_CPS1\n");
	    curr_fm_channels = 8;  // from YM2151
	    curr_adpcm_channels = 4;  // from MSM6295
	    break;

	case HARDWARE_CAPCOM_CPS1_QSOUND:	
	case HARDWARE_CAPCOM_CPS2:
		log_cb(RETRO_LOG_INFO, "detected QSOUND\n");
	    curr_adpcm_channels = 16;  // from QSound
	    break;
	
	case HARDWARE_CAVE_68K_ONLY:
		log_cb(RETRO_LOG_INFO, "detected CAVE_68K\n");
	    curr_adpcm_channels = 4*2;  // 2* OKIM6295 (1 may be unused)
	    break;

    }
    //TODO: if (BurnDrvGetHardwareCode() & HARDWARE_SEGA_YM2413) {
    //TODO: if (BurnDrvGetHardwareCode() & HARDWARE_SEGA_YM2203) {
    
    // 3rd check via parentrom/drvname (most reliable, used only for particular setups)
    const char * parentrom	= BurnDrvGetTextA(DRV_PARENT);
    const char * drvname	= BurnDrvGetTextA(DRV_NAME);
    //const char * boardrom	= BurnDrvGetTextA(DRV_BOARDROM);
    const char * system	= BurnDrvGetTextA(DRV_SYSTEM);
    if(!parentrom) parentrom="";
    if(!drvname) drvname="";
    if(!system) system="";
    log_cb(RETRO_LOG_INFO, "parentrom: %s \t drvname: %s \t system: %s \n", parentrom, drvname, system);
    
    /*
    //TODO: if system.startswith("GX") -> konami GX?
    // curr_adpcm_channels = 8*2;  // is using 2x 054539
    
    // mystwarr-based board
    const char* konami_mystwarr_arr[] = { "mystwarr", "gaiapols" }; // TODO: more http://www.system16.com/hardware.php?id=573&gid=847
    for(int i; i<(sizeof(konami_mystwarr_arr)/sizeof(konami_mystwarr_arr[0])); i++)
    {
	if ((parentrom && strcmp(parentrom, konami_mystwarr_arr[i]) == 0) || (drvname && strcmp(drvname, konami_mystwarr_arr[i]) == 0))
	{
	    curr_adpcm_channels = 8*2;  // 2* K054539
	}
    }*/
    
    // retrurn a different value according to the type param
    // TODO: return all at once with an array?
    switch(type)
    {
	case 1:
	    return(curr_fm_channels);
	case 2:
	    return(curr_adpcm_channels);
	case 3:
	    return(curr_psg_channels);
    }
}

int get_supported_fm_channels() { return(get_supported_sound_channels(1)); }
int get_supported_adpcm_channels() { return(get_supported_sound_channels(2)); }
int get_supported_psg_channels() { return(get_supported_sound_channels(3)); }


void set_environment()
{
	std::vector<const retro_core_option_v2_definition*> vars_systems;
	struct retro_core_option_v2_definition *option_defs_us;
#ifdef _MSC_VER
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
	#ifndef FORCE_USE_VFS
	#define FORCE_USE_VFS
    #endif
#endif
#endif

#ifdef FORCE_USE_VFS
	struct retro_vfs_interface_info vfs_iface_info;
#endif

	// Add the Global core options
	vars_systems.push_back(&var_fbneo_allow_depth_32);
	vars_systems.push_back(&var_fbneo_vertical_mode);
	vars_systems.push_back(&var_fbneo_force_60hz);
	vars_systems.push_back(&var_fbneo_allow_patched_romsets);
	vars_systems.push_back(&var_fbneo_analog_speed);
	vars_systems.push_back(&var_fbneo_lightgun_crosshair_emulation);
	vars_systems.push_back(&var_fbneo_cpu_speed_adjust);
#ifdef USE_CYCLONE
	vars_systems.push_back(&var_fbneo_cyclone);
#endif
	if (BurnDrvGetFlags() & BDF_HISCORE_SUPPORTED)
		vars_systems.push_back(&var_fbneo_hiscores);

	if (pgi_diag)
		vars_systems.push_back(&var_fbneo_diagnostic_input);

	if (bIsNeogeoCartGame)
	{
		// Add the Neo Geo core options
		if (allow_neogeo_mode)
			vars_systems.push_back(&var_fbneo_neogeo_mode);
		vars_systems.push_back(&var_fbneo_memcard_mode);
		if (pgi_debug_dip_1)
		{
			vars_systems.push_back(&var_fbneo_debug_dip_1_1);
			vars_systems.push_back(&var_fbneo_debug_dip_1_2);
			vars_systems.push_back(&var_fbneo_debug_dip_1_3);
			vars_systems.push_back(&var_fbneo_debug_dip_1_4);
			vars_systems.push_back(&var_fbneo_debug_dip_1_5);
			vars_systems.push_back(&var_fbneo_debug_dip_1_6);
			vars_systems.push_back(&var_fbneo_debug_dip_1_7);
			vars_systems.push_back(&var_fbneo_debug_dip_1_8);
		}
		if (pgi_debug_dip_2)
		{
			vars_systems.push_back(&var_fbneo_debug_dip_2_1);
			vars_systems.push_back(&var_fbneo_debug_dip_2_2);
			vars_systems.push_back(&var_fbneo_debug_dip_2_3);
			vars_systems.push_back(&var_fbneo_debug_dip_2_4);
			vars_systems.push_back(&var_fbneo_debug_dip_2_5);
			vars_systems.push_back(&var_fbneo_debug_dip_2_6);
			vars_systems.push_back(&var_fbneo_debug_dip_2_7);
			vars_systems.push_back(&var_fbneo_debug_dip_2_8);
		}
	}

	// Frameskip settings
	if (bLibretroSupportsAudioBuffStatus)
	{
		vars_systems.push_back(&var_fbneo_frameskip_type);
		vars_systems.push_back(&var_fbneo_frameskip_manual_threshold);
	}
	vars_systems.push_back(&var_fbneo_fixed_frameskip);

	// Audio settings
	if (nGameType != RETRO_GAME_TYPE_NEOCD)
		vars_systems.push_back(&var_fbneo_samplerate);
	vars_systems.push_back(&var_fbneo_sample_interpolation);
	vars_systems.push_back(&var_fbneo_fm_interpolation);
	vars_systems.push_back(&var_fbneo_lowpass_filter);

#ifdef FBNEO_DEBUG
	// Debug settings
	vars_systems.push_back(&var_fbneo_debug_layer_1);
	vars_systems.push_back(&var_fbneo_debug_layer_2);
	vars_systems.push_back(&var_fbneo_debug_layer_3);
	vars_systems.push_back(&var_fbneo_debug_layer_4);
	vars_systems.push_back(&var_fbneo_debug_sprite_1);
	vars_systems.push_back(&var_fbneo_debug_sprite_2);
	vars_systems.push_back(&var_fbneo_debug_sprite_3);
	vars_systems.push_back(&var_fbneo_debug_sprite_4);
	vars_systems.push_back(&var_fbneo_debug_sprite_5);
	vars_systems.push_back(&var_fbneo_debug_sprite_6);
	vars_systems.push_back(&var_fbneo_debug_sprite_7);
	vars_systems.push_back(&var_fbneo_debug_sprite_8);
#endif
	
	unsigned curr_fm_channels = get_supported_fm_channels();
	unsigned curr_adpcm_channels = get_supported_adpcm_channels();
	unsigned curr_psg_channels = get_supported_psg_channels();
	
	/*printf("curr_fm_channels:%d\n", curr_fm_channels);
	printf("curr_adpcm_channels:%d\n", curr_adpcm_channels);
	printf("curr_psg_channels:%d\n", curr_psg_channels);*/

	const struct retro_core_option_value volume_values[RETRO_NUM_CORE_OPTION_VALUES_MAX] = {
         { .value="0", .label=NULL },
         { .value="10", .label=NULL },
         { .value="20", .label=NULL },
         { .value="30", .label=NULL },
         { .value="40", .label=NULL },
         { .value="50", .label=NULL },
         { .value="60", .label=NULL },
         { .value="70", .label=NULL },
         { .value="80", .label=NULL },
         { .value="90", .label=NULL },
         { .value="100", .label=NULL },
         { .value=NULL, .label=NULL },
      };
	
	struct retro_core_option_v2_definition* curr_option = 0;
	char curr_option_key[100] = { 0 } ;  // "fbneo-fm-sound-channel-00-volume";
	char curr_option_desc[100] = { 0 } ; //"FM Sound chip Channel 00 Volume %";
	char curr_option_info[100] = { 0 } ; //"Reduce the volume of the current FM sound chip channel 0."
	
	for (unsigned c = 0; c < curr_fm_channels; c++)
	{
		curr_option = (struct retro_core_option_v2_definition*) malloc( sizeof( struct retro_core_option_v2_definition ) );
		// TODO: need to call free on core deinit
		snprintf( curr_option_key, 100, "fbneo-fm-sound-channel-%d-volume", c);
		snprintf( curr_option_desc, 100, "FM Sound chip Channel %d Volume %", c);
		snprintf( curr_option_info, 100, "Reduce the volume of the current FM sound chip channel %d.", c);
		curr_option->key = strdup(curr_option_key);
		curr_option->desc = strdup(curr_option_desc);
		curr_option->info = strdup(curr_option_info);
		// TODO: need to call free on core deinit
		curr_option->desc_categorized = NULL;
		curr_option->info_categorized = NULL;
		curr_option->category_key = "audio";
		curr_option->default_value = "100";
		memcpy(curr_option->values, volume_values, sizeof(volume_values));
	  vars_systems.push_back(curr_option);
	}
	
	for (unsigned c = 0; c < curr_adpcm_channels; c++)
	{
		curr_option = (struct retro_core_option_v2_definition*) malloc( sizeof( struct retro_core_option_v2_definition ) );
		// TODO: need to call free on core deinit
		snprintf( curr_option_key, 100, "fbneo-adpcm-sound-channel-%d-volume", c);
		snprintf( curr_option_desc, 100, "ADPCM Sound chip Channel %d Volume %", c);
		snprintf( curr_option_info, 100, "Reduce the volume of the current ADPCM sound chip channel %d.", c);
		curr_option->key = strdup(curr_option_key);
		curr_option->desc = strdup(curr_option_desc);
		curr_option->info = strdup(curr_option_info);
		// TODO: need to call free on core deinit
		curr_option->desc_categorized = NULL;
		curr_option->info_categorized = NULL;
		curr_option->category_key = "audio";
		curr_option->default_value = "100";
		memcpy(curr_option->values, volume_values, sizeof(volume_values));
	  vars_systems.push_back(curr_option);
	}
	
	for (unsigned c = 0; c < curr_psg_channels; c++)
	{
		curr_option = (struct retro_core_option_v2_definition*) malloc( sizeof( struct retro_core_option_v2_definition ) );
		// TODO: need to call free on core deinit
		snprintf( curr_option_key, 100, "fbneo-psg-sound-channel-%d-volume", c);
		snprintf( curr_option_desc, 100, "PSG Sound chip Channel %d Volume %", c);
		snprintf( curr_option_info, 100, "Reduce the volume of the current PSG sound chip channel %d.", c);
		curr_option->key = strdup(curr_option_key);
		curr_option->desc = strdup(curr_option_desc);
		curr_option->info = strdup(curr_option_info);
		// TODO: need to call free on core deinit
		curr_option->desc_categorized = NULL;
		curr_option->info_categorized = NULL;
		curr_option->category_key = "audio";
		curr_option->default_value = "100";
		memcpy(curr_option->values, volume_values, sizeof(volume_values));
	  vars_systems.push_back(curr_option);
	}

	int nbr_vars = vars_systems.size();
	int nbr_dips = dipswitch_core_options.size();
	int nbr_cheats = cheat_core_options.size();

#if 0
	log_cb(RETRO_LOG_INFO, "set_environment: SYSTEM: %d, DIPSWITCH: %d\n", nbr_vars, nbr_dips);
#endif

	option_defs_us = (struct retro_core_option_v2_definition*)calloc(nbr_vars + nbr_dips + nbr_cheats + 1, sizeof(struct retro_core_option_v2_definition));

	int idx_var = 0;

	// Add the System core options
	for (int i = 0; i < nbr_vars; i++, idx_var++)
	{
		option_defs_us[idx_var] = *vars_systems[i];
	}

	// Add the DIP switches core options
	for (int dip_idx = 0; dip_idx < nbr_dips; dip_idx++)
	{
		option_defs_us[idx_var].category_key     = "dipswitch";
		option_defs_us[idx_var].key              = dipswitch_core_options[dip_idx].option_name.c_str();
		option_defs_us[idx_var].desc             = dipswitch_core_options[dip_idx].friendly_name.c_str();
		option_defs_us[idx_var].desc_categorized = dipswitch_core_options[dip_idx].friendly_name_categorized.c_str();
		option_defs_us[idx_var].default_value    = dipswitch_core_options[dip_idx].default_bdi.szText;
		// Instead of filtering out the dips, make the description a warning if it's a neogeo game using a different default bios
		if (neogeo_use_specific_default_bios && bIsNeogeoCartGame && dipswitch_core_options[dip_idx].friendly_name.compare("[Dipswitch] BIOS") == 0)
			option_defs_us[idx_var].info         = "THIS NEOGEO GAME USES A DIFFERENT DEFAULT BIOS, CHANGE IT AT YOUR OWN RISK";
		else
			option_defs_us[idx_var].info         = "Dipswitch setting, setting is specific to the running romset. Some dipswitches require a restart to work properly.";
		for (int dip_value_idx = 0; dip_value_idx < dipswitch_core_options[dip_idx].values.size(); dip_value_idx++)
		{
			option_defs_us[idx_var].values[dip_value_idx].value = dipswitch_core_options[dip_idx].values[dip_value_idx].friendly_name.c_str();
		}
		option_defs_us[idx_var].values[dipswitch_core_options[dip_idx].values.size()].value = NULL;
		if (option_defs_us[idx_var].default_value == NULL) HandleMessage(RETRO_LOG_ERROR, "Default value for '%s' not found, it's a bug, please report it\n", option_defs_us[idx_var].desc);
		idx_var++;
	}

	// Add the cheats core options
	for (int cheat_idx = 0; cheat_idx < nbr_cheats; cheat_idx++)
	{
		option_defs_us[idx_var].category_key     = "cheat";
		option_defs_us[idx_var].key              = cheat_core_options[cheat_idx].option_name.c_str();
		option_defs_us[idx_var].desc             = cheat_core_options[cheat_idx].friendly_name.c_str();
		option_defs_us[idx_var].desc_categorized = cheat_core_options[cheat_idx].friendly_name_categorized.c_str();
		option_defs_us[idx_var].default_value    = cheat_core_options[cheat_idx].default_value.c_str();
		option_defs_us[idx_var].info             = "Specific to the running romset and your cheat database";
		for (int cheat_value_idx = 0; cheat_value_idx < cheat_core_options[cheat_idx].values.size(); cheat_value_idx++)
		{
			option_defs_us[idx_var].values[cheat_value_idx].value = cheat_core_options[cheat_idx].values[cheat_value_idx].friendly_name.c_str();
		}
		option_defs_us[idx_var].values[cheat_core_options[cheat_idx].values.size()].value = NULL;
		idx_var++;
	}

	option_defs_us[idx_var] = var_empty;

	static struct retro_core_option_v2_category option_cats_us[] =
	{
		{
			"neogeo",
			"Neo-Geo Settings",
			"Configure Neo-Geo Settings"
		},
		{
			"frameskip",
			"Frameskip Settings",
			"Configure Frameskip Settings"
		},
		{
			"audio",
			"Audio Settings",
			"Configure Audio Settings"
		},
		{
			"dipswitch",
			"DIP Switches",
			"Configure DIP Switches"
		},
		{
			"cheat",
			"Cheat",
			"Enable Cheats"
		},
#ifdef FBNEO_DEBUG
		{
			"debug",
			"Debug",
			"Configure Debug Features"
		},
#endif
		{ NULL, NULL, NULL },
	};

	struct retro_core_options_v2 options_us = {
		option_cats_us,
		option_defs_us
	};

	unsigned version = 0;

	if (!environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version))
		version = 0;

	if (version >= 2)
	{
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2, &options_us);
	}
	else
	{
		size_t i, j;
		size_t num_options                                     = 0;
		struct retro_core_option_definition *option_v1_defs_us = NULL;
		struct retro_variable *variables                       = NULL;
		char **values_buf                                      = NULL;

		/* Determine total number of options */
		while (true)
		{
			if (option_defs_us[num_options].key)
				num_options++;
			else
				break;
		}

		if (version >= 1)
		{
			/* Allocate US array */
			option_v1_defs_us = (struct retro_core_option_definition *)calloc(num_options + 1, sizeof(struct retro_core_option_definition));

			/* Copy parameters from option_defs_us array */
			for (i = 0; i < num_options; i++)
			{
				struct retro_core_option_v2_definition *option_def_us = &option_defs_us[i];
				struct retro_core_option_value *option_values         = option_def_us->values;
				struct retro_core_option_definition *option_v1_def_us = &option_v1_defs_us[i];
				struct retro_core_option_value *option_v1_values      = option_v1_def_us->values;

				option_v1_def_us->key           = option_def_us->key;
				option_v1_def_us->desc          = option_def_us->desc;
				option_v1_def_us->info          = option_def_us->info;
				option_v1_def_us->default_value = option_def_us->default_value;

				/* Values must be copied individually... */
				while (option_values->value)
				{
					option_v1_values->value = option_values->value;
					option_v1_values->label = option_values->label;

					option_values++;
					option_v1_values++;
				}
			}

			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, option_v1_defs_us);
		}
		else
		{

			/* Allocate arrays */
			variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
			values_buf = (char **)calloc(num_options, sizeof(char *));

			if (!variables || !values_buf)
				goto error;

			/* Copy parameters from option_defs_us array */
			for (i = 0; i < num_options; i++)
			{
				const char *key                        = option_defs_us[i].key;
				const char *desc                       = option_defs_us[i].desc;
				const char *default_value              = option_defs_us[i].default_value;
				struct retro_core_option_value *values = option_defs_us[i].values;
				size_t buf_len                         = 3;
				size_t default_index                   = 0;

				values_buf[i] = NULL;

				if (desc)
				{
					size_t num_values = 0;

					/* Determine number of values */
					while (true)
					{
						if (values[num_values].value)
						{
							/* Check if this is the default value */
							if (default_value)
								if (strcmp(values[num_values].value, default_value) == 0)
									default_index = num_values;

							buf_len += strlen(values[num_values].value);
							num_values++;
						}
						else
							break;
					}

					if (num_values > 0)
					{

						buf_len += num_values - 1;
						buf_len += strlen(desc);

						values_buf[i] = (char *)calloc(buf_len, sizeof(char));
						if (!values_buf[i])
							goto error;

						strcpy(values_buf[i], desc);
						strcat(values_buf[i], "; ");

						/* Default value goes first */
						strcat(values_buf[i], values[default_index].value);

						/* Add remaining values */
						for (j = 0; j < num_values; j++)
						{
							if (j != default_index)
							{
								strcat(values_buf[i], "|");
								strcat(values_buf[i], values[j].value);
							}
						}
					}
				}

				variables[i].key   = key;
				variables[i].value = values_buf[i];
			}

			/* Set variables */
			environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

		}

error:
		/* Clean up */

		if (option_defs_us)
		{
			free(option_defs_us);
			option_defs_us = NULL;
		}

		if (option_v1_defs_us)
		{
			free(option_v1_defs_us);
			option_v1_defs_us = NULL;
		}

		if (values_buf)
		{
			for (i = 0; i < num_options; i++)
			{
				if (values_buf[i])
				{
					free(values_buf[i]);
					values_buf[i] = NULL;
				}
			}

			free(values_buf);
			values_buf = NULL;
		}

		if (variables)
		{
			free(variables);
			variables = NULL;
		}
	}

	// Initialize VFS
	// Only on UWP for now, since EEPROM saving is not VFS aware
#ifdef FORCE_USE_VFS
	vfs_iface_info.required_interface_version = FILESTREAM_REQUIRED_VFS_VERSION;
	vfs_iface_info.iface                      = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &vfs_iface_info))
		filestream_vfs_init(&vfs_iface_info);
#endif
}

static int percent_parser(const char *value)
{
	INT32 nVal = atoi(value);
	if (nVal == 0)
		nVal = 100;

	return (int)((double)nVal * 256.0 / 100.0 + 0.5);
}

void check_variables(void)
{
	struct retro_variable var = {0};

	var.key = var_fbneo_cpu_speed_adjust.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		nBurnCPUSpeedAdjust = percent_parser(var.value);
	}

	var.key = var_fbneo_allow_depth_32.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bAllowDepth32 = true;
		else
			bAllowDepth32 = false;
	}

	var.key = var_fbneo_vertical_mode.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nVerticalMode = 1;
		else if (strcmp(var.value, "alternate") == 0)
			nVerticalMode = 2;
		else if (strcmp(var.value, "TATE") == 0)
			nVerticalMode = 3;
		else if (strcmp(var.value, "TATE alternate") == 0)
			nVerticalMode = 4;
		else
			nVerticalMode = 0;
	}

	var.key = var_fbneo_force_60hz.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bForce60Hz = true;
		else
			bForce60Hz = false;
	}

	if (bLibretroSupportsAudioBuffStatus)
	{
		var.key = var_fbneo_frameskip_type.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "disabled") == 0)
				nFrameskipType = 0;
			else if (strcmp(var.value, "Fixed") == 0)
				nFrameskipType = 1;
			else if (strcmp(var.value, "Auto") == 0)
				nFrameskipType = 2;
			else if (strcmp(var.value, "Manual") == 0)
				nFrameskipType = 3;
		}

		var.key = var_fbneo_frameskip_manual_threshold.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			nFrameskipThreshold = strtol(var.value, NULL, 10);
	}

	var.key = var_fbneo_fixed_frameskip.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "0") == 0)
			nFrameskip = 1;
		else if (strcmp(var.value, "1") == 0)
			nFrameskip = 2;
		else if (strcmp(var.value, "2") == 0)
			nFrameskip = 3;
		else if (strcmp(var.value, "3") == 0)
			nFrameskip = 4;
		else if (strcmp(var.value, "4") == 0)
			nFrameskip = 5;
		else if (strcmp(var.value, "5") == 0)
			nFrameskip = 6;
	}

	if (pgi_diag)
	{
		var.key = var_fbneo_diagnostic_input.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "Hold Start") == 0)
			{
				diag_input = diag_input_start;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Start + A + B") == 0)
			{
				diag_input = diag_input_start_a_b;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, "Hold Start + A + B") == 0)
			{
				diag_input = diag_input_start_a_b;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Start + L + R") == 0)
			{
				diag_input = diag_input_start_l_r;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, "Hold Start + L + R") == 0)
			{
				diag_input = diag_input_start_l_r;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Hold Select") == 0)
			{
				diag_input = diag_input_select;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Select + A + B") == 0)
			{
				diag_input = diag_input_select_a_b;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, "Hold Select + A + B") == 0)
			{
				diag_input = diag_input_select_a_b;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Select + L + R") == 0)
			{
				diag_input = diag_input_select_l_r;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, "Hold Select + L + R") == 0)
			{
				diag_input = diag_input_select_l_r;
				SetDiagInpHoldFrameDelay(60);
			}
			else
			{
				diag_input = NULL;
				SetDiagInpHoldFrameDelay(0);
			}
		}
	}

	if (bIsNeogeoCartGame)
	{
		if (allow_neogeo_mode)
		{
			var.key = var_fbneo_neogeo_mode.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "MVS_EUR") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_EUR;
				else if (strcmp(var.value, "MVS_USA") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_USA;
				else if (strcmp(var.value, "MVS_JAP") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_JAP;
				else if (strcmp(var.value, "AES_EUR") == 0)
					g_opt_neo_geo_mode = NEOGEO_AES | NEOGEO_EUR;
				else if (strcmp(var.value, "AES_JAP") == 0)
					g_opt_neo_geo_mode = NEOGEO_AES | NEOGEO_JAP;
				else if (strcmp(var.value, "UNIBIOS") == 0)
					g_opt_neo_geo_mode = NEOGEO_UNI;
				else
					g_opt_neo_geo_mode = 0;
			}
		}
		var.key = var_fbneo_memcard_mode.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "disabled") == 0)
				nMemcardMode = 0;
			else if (strcmp(var.value, "shared") == 0)
				nMemcardMode = 1;
			else if (strcmp(var.value, "per-game") == 0)
				nMemcardMode = 2;
		}
		if (pgi_debug_dip_1)
		{
			var.key = var_fbneo_debug_dip_1_1.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x01;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x01;
			}
			var.key = var_fbneo_debug_dip_1_2.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x02;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x02;
			}
			var.key = var_fbneo_debug_dip_1_3.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x04;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x04;
			}
			var.key = var_fbneo_debug_dip_1_4.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x08;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x08;
			}
			var.key = var_fbneo_debug_dip_1_5.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x10;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x10;
			}
			var.key = var_fbneo_debug_dip_1_6.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x20;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x20;
			}
			var.key = var_fbneo_debug_dip_1_7.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x40;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x40;
			}
			var.key = var_fbneo_debug_dip_1_8.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x80;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x80;
			}
			pgi_debug_dip_1->Input.nVal = pgi_debug_dip_1->Input.Constant.nConst;
			if (pgi_debug_dip_1->Input.pVal)
				*(pgi_debug_dip_1->Input.pVal) = pgi_debug_dip_1->Input.nVal;
		}
		if (pgi_debug_dip_2)
		{
			var.key = var_fbneo_debug_dip_2_1.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x01;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x01;
			}
			var.key = var_fbneo_debug_dip_2_2.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x02;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x02;
			}
			var.key = var_fbneo_debug_dip_2_3.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x04;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x04;
			}
			var.key = var_fbneo_debug_dip_2_4.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x08;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x08;
			}
			var.key = var_fbneo_debug_dip_2_5.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x10;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x10;
			}
			var.key = var_fbneo_debug_dip_2_6.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x20;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x20;
			}
			var.key = var_fbneo_debug_dip_2_7.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x40;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x40;
			}
			var.key = var_fbneo_debug_dip_2_8.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x80;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x80;
			}
			pgi_debug_dip_2->Input.nVal = pgi_debug_dip_2->Input.Constant.nConst;
			if (pgi_debug_dip_2->Input.pVal)
				*(pgi_debug_dip_2->Input.pVal) = pgi_debug_dip_2->Input.nVal;
		}
	}

	if (BurnDrvGetFlags() & BDF_HISCORE_SUPPORTED)
	{
		var.key = var_fbneo_hiscores.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "enabled") == 0)
				EnableHiscores = true;
			else
				EnableHiscores = false;
		}
	}
	else
	{
		EnableHiscores = false;
	}

	var.key = var_fbneo_allow_patched_romsets.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bPatchedRomsetsEnabled = true;
		else
			bPatchedRomsetsEnabled = false;
	}

	if (nGameType != RETRO_GAME_TYPE_NEOCD)
	{
		var.key = var_fbneo_samplerate.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "48000") == 0)
				g_audio_samplerate = 48000;
			else if (strcmp(var.value, "44100") == 0)
				g_audio_samplerate = 44100;
			else
				g_audio_samplerate = 48000;
		}
	}
	else
	{
		// src/burn/drv/neogeo/neo_run.cpp is mentioning issues with ngcd cdda playback if samplerate isn't 44100
		g_audio_samplerate = 44100;
	}

	var.key = var_fbneo_sample_interpolation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "4-point 3rd order") == 0)
			nInterpolation = 3;
		else if (strcmp(var.value, "2-point 1st order") == 0)
			nInterpolation = 1;
		else if (strcmp(var.value, "disabled") == 0)
			nInterpolation = 0;
		else
			nInterpolation = 3;
	}

	var.key = var_fbneo_fm_interpolation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "4-point 3rd order") == 0)
			nFMInterpolation = 3;
		else if (strcmp(var.value, "disabled") == 0)
			nFMInterpolation = 0;
		else
			nFMInterpolation = 3;
	}

	var.key = var_fbneo_lowpass_filter.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bLowPassFilterEnabled = true;
		else
			bLowPassFilterEnabled = false;
	}

	var.key = var_fbneo_analog_speed.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		nAnalogSpeed = percent_parser(var.value);
	}

	var.key = var_fbneo_lightgun_crosshair_emulation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "hide with lightgun device") == 0)
			nLightgunCrosshairEmulation = 0;
		else if (strcmp(var.value, "always hide") == 0)
			nLightgunCrosshairEmulation = 1;
		else if (strcmp(var.value, "always show") == 0)
			nLightgunCrosshairEmulation = 2;
		RefreshLightgunCrosshair();
	}

#ifdef USE_CYCLONE
	var.key = var_fbneo_cyclone.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bCycloneEnabled = true;
		else if (strcmp(var.value, "disabled") == 0)
			bCycloneEnabled = false;
	}
#endif

	unsigned curr_fm_channels = get_supported_fm_channels();
	unsigned curr_adpcm_channels = get_supported_adpcm_channels();
	unsigned curr_psg_channels = get_supported_psg_channels();

	if (curr_fm_channels>0)
	{
		char fbneo_fm_sound_channel_volume_key[] = "fbneo-fm-sound-channel-0-volume";
		var.key = fbneo_fm_sound_channel_volume_key;
		for (unsigned c = 0; c < curr_fm_channels; c++)
		{
			fbneo_fm_sound_channel_volume_key[23] = c+'0';
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var))
				nBurnFMSoundChannelVolumes[c] = atoi(var.value);
		}
	}

	if (curr_adpcm_channels>0)
	{
		char fbneo_adpcm_sound_channel_volume_key[40] = "fbneo-adpcm-sound-channel-0-volume";
		var.key = fbneo_adpcm_sound_channel_volume_key;
		for (unsigned c = 0; c < curr_adpcm_channels; c++)
		{
			//fbneo_adpcm_sound_channel_volume_key[26] = c+'0';
			sprintf( fbneo_adpcm_sound_channel_volume_key, "fbneo-adpcm-sound-channel-%u-volume", c );
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var))
				nBurnADPCMSoundChannelVolumes[c] = atoi(var.value);
		}
	}
	
	if (curr_psg_channels>0)
	{
	    char fbneo_psg_sound_channel_volume_key[] = "fbneo-psg-sound-channel-0-volume";
	    var.key = fbneo_psg_sound_channel_volume_key;
	    for (unsigned c = 0; c < curr_psg_channels; c++) {
		    fbneo_psg_sound_channel_volume_key[24] = c+'0';
		    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var))
		    {
			    nBurnPSGSoundChannelVolumes[c] = atoi(var.value);
		    }
	    }
	}
	
#ifdef FBNEO_DEBUG
	var.key = var_fbneo_debug_layer_1.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nBurnLayer |= 1;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~1;
	}

	var.key = var_fbneo_debug_layer_2.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nBurnLayer |= 2;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~2;
	}

	var.key = var_fbneo_debug_layer_3.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nBurnLayer |= 4;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~4;
	}

	var.key = var_fbneo_debug_layer_4.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nBurnLayer |= 8;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~8;
	}

	var.key = var_fbneo_debug_sprite_1.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x01;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x01;
	}

	var.key = var_fbneo_debug_sprite_2.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x02;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x02;
	}

	var.key = var_fbneo_debug_sprite_3.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x04;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x04;
	}

	var.key = var_fbneo_debug_sprite_4.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x08;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x08;
	}

	var.key = var_fbneo_debug_sprite_5.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x10;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x10;
	}

	var.key = var_fbneo_debug_sprite_6.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x20;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x20;
	}

	var.key = var_fbneo_debug_sprite_7.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x40;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x40;
	}

	var.key = var_fbneo_debug_sprite_8.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			nSpriteEnable |= 0x80;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x80;
	}
#endif
}

#ifdef USE_CYCLONE
void SetSekCpuCore()
{
	nSekCpuCore = (bCycloneEnabled ? 0 : 1);
}
#endif
