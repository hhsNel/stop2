/* user-defined menu */
struct dspelement help_menu[] = {
/*	 element type        argument                                                   input */
	{EL_PLAINTEXT,       {.v="some text"},                                          NULL},
};

/* main menu */
struct dspelement main_menu[] = {
/*	 element type        argument                                                   input          foreground */
	{EL_MENU_BUTTON,    {.v="menu btn",.i=sizeof(help_menu)/sizeof(*help_menu)},    help_menu,     {.color_mode=CLRM_USR,.clr=7}},
	{EL_GRAPH_OVER_TIME,{0},                                                        vcpu_load,     {.color_mode=CLRM_USR,.clr=7}},
	{EL_DIVIDER,        {.i=4},                                                     NULL,          {.color_mode=CLRM_USR,.clr=7}},
		{EL_VALUE,      {.v="%d"},                                                  vram_total,    {.color_mode=CLRM_USR,.clr=7}},
		{EL_PLAINTEXT,  {.v="out of"},                                              NULL,          {.color_mode=CLRM_USR,.clr=7}},
		{EL_VALUE,      {.v="%d"},                                                  vram_max,      {.color_mode=CLRM_USR,.clr=7}},
		{EL_PLAINTEXT,  {.v="bytes"},                                               NULL,          {.color_mode=CLRM_USR,.clr=7}},
	{EL_PLAINTEXT,      {.v="sTop poc v0.0.1"},                                     NULL,          {.color_mode=CLRM_USR,.clr=7}},
	{EL_H_CAP,          {.i=1},                                                     NULL,          {.color_mode=CLRM_USR,.clr=7}},
		{EL_PLAINTEXT,  {.v="This is only one line"},                               NULL,          {.color_mode=CLRM_USR,.clr=7}},
};

#define OUT_BUFF_SIZE (65536)

struct color def_foreground = {
	.color_mode = CLRM_USR,
	.clr = 0
};
#define BG_CHAR ' '

#define POLL_TIMEOUT 5

