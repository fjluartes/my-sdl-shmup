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
} App;

typedef struct {
    int x;
    int y;
    SDL_Texture *texture;
} Entity;
