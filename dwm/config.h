/*   See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 15;        /* gaps between windows */
static const unsigned int snap      = 13;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 460;       /* horizontal padding of bar */
static const int user_bh            = 14;
static const char *barlayout        = "slt";
static const char *fonts[]          = { "Iosevka:size=10:" };
static const char dmenufont[]       = "BlexMono NF:size=13";
static const char col_gray1[]       = "#1f1f28";
static const char col_gray2[]       = "#dcd7ba";
static const char col_gray3[]       = "#dcd7ba";
static const char col_gray4[]       = "#dcd7ba";
static const char col_cyan[]        = "#1f1f28";
static const char *colors[][4]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_cyan/*col_gray2*/ },
	[SchemeSel]  = { col_gray1, col_gray4,  col_gray2  },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI" /*"6", "7", "8", "9"*/ };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      	     instance    title    tags mask     isfloating   CenterThisWindow?     monitor */
	//{ "XTerm",           NULL,       NULL,    0,            0,     	     1,		           -1 },
	//{ "Chromium",        NULL,       NULL,    0,            0,     	     1,		           -1 },
	{ "Pcmanfm",         NULL,       NULL,    0,            0,     	     1,		           -1 },
	{ "Gimp",            NULL,       NULL,    0,            1,           0,                    -1 },
	{ "Firefox",         NULL,       NULL,    1 << 8,       0,           0,                    -1 },
};

#include "fibonacci.c"

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "TILE",      tile },    /* first entry is default */
	{ "FLOAT",      NULL },    /* no layout function means floating behavior */
	{ "MONO",      monocle },
	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },

};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
// dmenu_r
// un -X 275 -Y 150 -W 860 -h 40 -l 10 -g 3 -bw 15 -fn "BlexMono NF:style=italic"
static const char *dmenucmd[] = { "dmenu_run", 
  "-X", "0", 
  "-Y", "0",
  "-W", "360",
  "-h", "40",
  "-l", "17",
  "-bw", "25",
  "-fn", "BlexMono NF:style=italic",
  NULL };
static const char *termcmd[]  = { "xterm", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY/*|ShiftMask*/,         XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_backslash, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY/*|ShiftMask*/,         XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	{ MODKEY|ShiftMask,             XK_m,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_n,  setgaps,        {.i = 0  } },
  { MODKEY,                       XK_minus,    spawn,        SHCMD("pamixer --allow-boost -d 5 && notifyvol") },
  { MODKEY,                       XK_equal,    spawn,        SHCMD("pamixer --allow-boost -i 5 && notifyvol") },
  { MODKEY,                       XK_v,        setgaps,        {.i = -1} },
  { MODKEY|ShiftMask,             XK_v,        setgaps,        {.i = +1} },
  { MODKEY,                       XK_r,        spawn,        SHCMD("dunstctl context") },
  { MODKEY|ShiftMask,             XK_s,        spawn,        SHCMD("scrot --select -z -b -e 'macstyle.sh $f'") },
  { MODKEY|ShiftMask,             XK_p,        spawn,        SHCMD("dmenu-power.sh") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

