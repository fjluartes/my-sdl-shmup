/*
 * Copyright (C) 2015-2018,2022 Parallel Realities. All rights reserved.
 */

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window   *window;
	int up;
	int down;
	int left;
	int right;
	int fire;
} App;

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int health;
    SDL_Texture *texture;
} Entity;
