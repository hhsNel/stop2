/* user-defined menu */
struct dspelement help_menu[] = {
/*	 element type        argument                                                   input */
	{EL_PLAINTEXT,       {.v="some text"},                                          NULL},
};

/* main menu */
struct dspelement main_menu[] = {
/*	 element type        argument                                                   input */
	{EL_MENU_BUTTON,    {.v="menu btn",.i=sizeof(help_menu)/sizeof(*help_menu)},    help_menu},
	{EL_GRAPH_OVER_TIME,{0},                                                        vcpu_load},
	{EL_DIVIDER,        {.i=2},                                                     NULL},
		{EL_VALUE,      {.v="%d"},                                                  vram_max},
		{EL_VALUE,      {.v="%d"},                                                  vram_total},
	{EL_PLAINTEXT,      {.v="sTop poc v0.0.1"},                                     NULL},
	{EL_H_CAP,          {.i=1},                                                     NULL},
		{EL_PLAINTEXT,  {.v="This is only one line"},                               NULL},
};
