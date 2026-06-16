/*
 * Copyright (C) 2015-2018,2022 Parallel Realities. All rights reserved.
 */

#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

App app;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}
