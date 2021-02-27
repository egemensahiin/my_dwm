/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappx     = 8;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Inconsolata:size=10", "Font Awesome 5 Free Regular:size=10", "Font Awesome 5 Brands Regular:size=10", "Font Awesome 5 Free Solid:size=10", "Inconsolata Nerd Font:size=10" };
static const char dmenufont[]       = "Inconsolata:size=10";
static const char col_gray1[]       = "#282828";/*"#ebdbb2";*/
static const char col_gray2[]       = "#d5c4a1";
static const char col_gray3[]       = "#bdae93";
static const char col_gray4[]       = "#a89984";
static const char col_cyan[]        = "#fabd2f";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         			 instance    title       tags mask     isfloating   monitor */
	{ "Gimp",        			  NULL,       NULL,      	1 << 4,       0,           -1 },
	{ "Pavucontrol", 			  NULL,       NULL,      	0,            1,           -1 },
	{ "Firefox",     			  NULL,       NULL,      	1 << 1,       0,           -1 },
	{ "qutebrowser", 			  NULL,       NULL,      	1 << 1,       0,           -1 },
	{ "discord",     			  NULL,       NULL,      	1 << 6,       0,           -1 },
	{ "PyMOL",	     			  NULL,       NULL,      	1 << 7,       0,           -1 },
	{ "Mpv",	     			  NULL,       NULL,      	1 << 4,       0,           -1 },
	{ "VSCodium",     			  NULL,       NULL,      	1 << 2,       0,           -1 },
	{ "VirtualBox Manager", 	  NULL,       NULL,			1 << 8,       0,           -1 },
	{ "libreoffice-startcenter",  NULL,       NULL,			1 << 5,       0,           -1 },
	{ NULL,  					  NULL,       "spotify-tui",1 << 3,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static const char *dmenucmd[]   = { "dmenu_run", /* "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray1,*/ NULL };
static const char *termcmd[]    = { "st", NULL };
static const char *roficmd[]    = { "rofi", "-show", "drun", NULL };
static const char *wroficmd[]   = { "/home/egemen/.local/bin/rofi_web.sh", NULL };
static const char *yroficmd[]   = { "/home/egemen/.local/bin/rofi_ytb.sh", NULL };
static const char *codecmd[]    = { "codium", NULL };
static const char *vimcmd[]     = { "st", "-e", "vim", NULL };
static const char *sptcmd[]     = { "st", "-e", "spt", NULL };
static const char *rangercmd[]  = { "st", "-e", "ranger", NULL };
static const char *firefoxcmd[] = { "primusrun", "firefox", NULL };
static const char *browsecmd[]  = { "qutebrowser", NULL };
static const char *audiocmd[]   = { "pavucontrol", NULL };
static const char *officecmd[]  = { "libreoffice", NULL };
static const char *mendelcmd[]  = { "mendeleydesktop", NULL };
static const char *gimpcmd[]    = { "primusrun", "gimp", NULL };
static const char *locker[]     = { "/home/egemen/.local/bin/locker.sh", NULL };
static const char *power[]      = { "/home/egemen/.local/bin/powermenu.sh", NULL };
static const char *screenshot[] = { "/home/egemen/.local/bin/screenshot.sh", NULL };
static const char *incvol[]     = { "amixer", "set", "Master", "5%+", NULL  };
static const char *decvol[]     = { "amixer", "set", "Master", "5%-", NULL  };
static const char *mutevol[]    = { "amixer", "-q", "set", "Master", "toggle", NULL  };
static const char *incbri[]     = { "brightnessctl", "set", "10%+", "&&", NULL };
static const char *decbri[]     = { "brightnessctl", "--min-value=750", "set", "10%-", NULL };
static const char *vboxcmd[]    = { "virtualbox", NULL };
static const char *playcmd[]    = { "playerctl", "--player=spotifyd,%any", "play-pause", NULL };

static Key keys[] = {
	/* modifier         key                      function        argument */
	{ MODKEY,           XK_d,                    spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask, XK_d,                    spawn,          {.v = wroficmd } },
	{ MODKEY,		    XK_y,                    spawn,          {.v = yroficmd } },
	{ MODKEY,           XK_a,                    spawn,          {.v = audiocmd } },
	{ MODKEY,           XK_g,                    spawn,          {.v = gimpcmd } },
	{ MODKEY,           XK_b,                    spawn,          {.v = browsecmd } },
	{ MODKEY,           XK_f,                    spawn,          {.v = firefoxcmd } },
	{ MODKEY,           XK_v,                    spawn,          {.v = vimcmd } },
	{ MODKEY,           XK_s,                    spawn,          {.v = sptcmd } },
	{ MODKEY,           XK_r,                    spawn,          {.v = rangercmd } },
	{ MODKEY,           XK_c,                    spawn,          {.v = codecmd } },
	{ MODKEY,           XK_x,                    spawn,          {.v = locker } },
	{ MODKEY,           XK_o,                    spawn,          {.v = vboxcmd } },
	{ MODKEY,           XK_n,                    spawn,          {.v = officecmd } },
	{ MODKEY,           XK_m,                    spawn,          {.v = mendelcmd } },
	{ 0,                XF86XK_AudioLowerVolume, spawn,          {.v = decvol } },
	{ 0,                XF86XK_AudioPlay, 		 spawn,          {.v = playcmd } },
	{ 0,                XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                XF86XK_AudioRaiseVolume, spawn,          {.v = incvol } },
	{ 0,                XF86XK_MonBrightnessDown,spawn,          {.v = decbri } },
	{ 0,                XF86XK_MonBrightnessUp,  spawn,          {.v = incbri } },
	{ 0,                XK_Print,                spawn,          {.v = screenshot } },
	{ MODKEY,          	XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask, XK_b,                    togglebar,      {0} },
	{ MODKEY,           XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,           XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY,           XK_Up,                   focusstack,     {.i = +1 } },
	{ MODKEY,           XK_Down,                 focusstack,     {.i = -1 } },
	{ MODKEY,           XK_Right,                focusstack,     {.i = +1 } },
	{ MODKEY,           XK_Left,                 focusstack,     {.i = -1 } },
	{ MODKEY,           XK_i,                    incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_p,                    incnmaster,     {.i = -1 } },
	{ MODKEY,           XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,           XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask, XK_Return,               zoom,           {0} },
	{ MODKEY,           XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask, XK_q,                    killclient,     {0} },
	{ MODKEY|ShiftMask, XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask, XK_f,                    setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask, XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           XK_space,                setlayout,      {0} },
	{ MODKEY|ShiftMask, XK_space,                togglefloating, {0} },
	{ MODKEY,           XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,           XK_KP_Subtract,          setgaps,        {.i = -1 } },
	{ MODKEY,           XK_KP_Add,               setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask, XK_KP_Add,               setgaps,        {.i = 0  } },
	{ MODKEY,           XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period,               tagmon,         {.i = +1 } },
	TAGKEYS(            XK_1,                                    0)
	TAGKEYS(            XK_2,                                    1)
	TAGKEYS(            XK_3,                                    2)
	TAGKEYS(            XK_4,                                    3)
	TAGKEYS(            XK_5,                                    4)
	TAGKEYS(            XK_6,                                    5)
	TAGKEYS(            XK_7,                                    6)
	TAGKEYS(            XK_8,                                    7)
	TAGKEYS(            XK_9,                                    8)
	/*{ MODKEY|ShiftMask, XK_e,                    quit,           {0} },*/
	{ MODKEY|ShiftMask, XK_e,                    spawn,          {.v = power} },
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

