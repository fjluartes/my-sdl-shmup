/*
 * Copyright (C) 2015-2018,2022 Parallel Realities. All rights reserved.
 */

#include "common.h"

#include "defs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"

App app;
Entity player;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
	memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

	initSDL();

    player.x = SCREEN_WIDTH - 800;
    player.y = SCREEN_HEIGHT - 600;
    player.texture = loadTexture("gfx/player_small.png");

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

        blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}
