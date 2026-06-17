/*
 * Copyright (C) 2015-2018,2022 Parallel Realities. All rights reserved.
 * Revision 2026-06-17 fjluartes
 */

#include "common.h"

#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "stage.h"

App app;
Stage stage;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    long then;
    float remainder;
	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initStage();

	then = SDL_GetTicks();

	remainder = 0;

	while (1)
	{
		prepareScene();

		doInput();

		app.delegate.logic();

		app.delegate.draw();

		presentScene();

		capFrameRate(&then, &remainder);
	}

	return 0;
}

static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;
    wait = 16 + *remainder;
    *remainder -= SDL_GetTicks() - *then;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;
    if (wait < 1) wait = 1;
    SDL_Delay(wait);
    *remainder += 0.667;
    *then = SDL_GetTicks();
}
