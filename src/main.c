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
    player.texture = loadTexture("gfx/player.png");

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();

		if (app.up) {
            player.y -= 8;
        }
        else if (app.down) {
            player.y += 8;
        }
		if (app.left) {
            player.x -= 8;
        }
        else if (app.right) {
            player.x += 8;
        }
		// Restrict player to screen bounds
		if (player.y < 0) player.y = 0;
		else if (player.y > SCREEN_HEIGHT - 8) player.y = SCREEN_HEIGHT - 8;
		else if (player.x < 0) player.x = 0;
		else if (player.x > SCREEN_WIDTH - 8) player.x = SCREEN_WIDTH - 8;

        blit(player.texture, player.x, player.y);

		presentScene();

		SDL_Delay(16);
	}

	return 0;
}
