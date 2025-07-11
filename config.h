/* declarations */
struct dspelement help_menu[];
struct dspelement gate_menu[];
struct dspelement main_menu[];
#define HELP_LEN 2
#define GATE_LEN 1
#define MAIN_LEN 15

/* user-defined menu */
struct dspelement help_menu[] = {
/*	 element type        argument                         input          foreground                          background */
	{EL_PLAINTEXT,       {.v="some text"},                NULL,          {.color_mode=CLRM_USR,.clr=3},      {.color_mode=CLRM_USR,.clr=0}},
	{EL_MENU_BUTTON,    {.v="go back",.i=MAIN_LEN},       main_menu,     {.color_mode=CLRM_USR,.clr=5},      {.color_mode=CLRM_USR,.clr=3}},
};

/* another one */
struct dspelement gate_menu[] = {
/*	 element type        argument                         input          foreground                          background */
	{EL_MENU_BUTTON,    {.v="go to help",.i=HELP_LEN},    help_menu,     {.color_mode=CLRM_USR,.clr=5},      {.color_mode=CLRM_USR,.clr=3}},
};

/* main menu */
struct dspelement main_menu[] = {
/*	 element type        argument                         input          foreground                          background */
	{EL_DIVIDER,        {.i=2},                           NULL,          {},                                 {}},
		{EL_MENU_BUTTON,    {.v="menu btn",.i=HELP_LEN},  help_menu,     {.color_mode=CLRM_USR,.clr=5},      {.color_mode=CLRM_USR,.clr=3}},
		{EL_MENU_BUTTON,    {.v="gate",.i=GATE_LEN},      gate_menu,     {.color_mode=CLRM_USR,.clr=5},      {.color_mode=CLRM_USR,.clr=3}},
	{EL_GRAPH_OVER_TIME,{0},                              vcpu_load,     {.color_mode=CLRM_USR,.clr=7},      {.color_mode=CLRM_USR,.clr=0}},
	{EL_DIVIDER,        {.i=5},                           NULL,          {},                                 {}},
		{EL_VALUE,      {.v="%d"},                        vram_total,    {.color_mode=CLRM_USR,.clr=4},      {.color_mode=CLRM_USR,.clr=0}},
		{EL_PLAINTEXT,  {.v="out of"},                    NULL,          {.color_mode=CLRM_USR,.clr=4},      {.color_mode=CLRM_USR,.clr=0}},
		{EL_VALUE,      {.v="%d"},                        vram_max,      {.color_mode=CLRM_USR,.clr=4},      {.color_mode=CLRM_USR,.clr=0}},
		{EL_PLAINTEXT,  {.v="bytes"},                     NULL,          {.color_mode=CLRM_USR,.clr=4},      {.color_mode=CLRM_USR,.clr=0}},
		{EL_DIVIDER,    {.i=2},                           NULL,          {},                                 {}},
			{EL_PLAINTEXT,{.v="This is"},                 NULL,          {.color_mode=CLRM_USR,.clr=6},      {.color_mode=CLRM_USR,.clr=0}},
			{EL_PLAINTEXT,{.v="stacked text"},            NULL,          {.color_mode=CLRM_USR,.clr=6},      {.color_mode=CLRM_USR,.clr=0}},
	{EL_PLAINTEXT,      {.v="sTop poc v0.0.1"},           NULL,          {.color_mode=CLRM_USR,.clr=1},      {.color_mode=CLRM_USR,.clr=0}},
	{EL_H_CAP,          {.i=1},                           NULL,          {},                                 {}},
		{EL_PLAINTEXT,  {.v="This is only one line"},     NULL,          {.color_mode=CLRM_USR,.clr=7},      {.color_mode=CLRM_USR,.clr=0}},
};

/* size of the termctl.h output buffer. Worst case scenario, each character can be 19 bytes */
#define OUT_BUFF_SIZE (65536)

/* default foreground character, it's color, and it's background color */
struct color def_foreground = {
	.color_mode = CLRM_USR,
	.clr = 0
};
struct color def_background = {
	.color_mode = CLRM_USR,
	.clr = 0
};
#define BG_CHAR ' '

/* override for the selected element. CLRM_UNSET keeps the element's default */
struct color selected_foreground = {
	.color_mode = CLRM_UNSET
};
struct color selected_background = {
	.color_mode = CLRM_USR,
	.clr = 4
};

/* timeout used for poll call, milis */
#define POLL_TIMEOUT 125 /* 125 miliseconds = 4 updates per second */

/* keybinds */
struct key keybinds[] = {
/*   char    action */
	{'q',    quit},
	{'j',    go_down},
	{'k',    go_up},
	{'\n',   interact},
};

