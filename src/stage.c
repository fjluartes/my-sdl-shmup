/*
 * stage.c: file that handles actual game logic
 */
#include "common.h"

#include "draw.h"
#include "stage.h"

extern App app;
extern Stage stage;

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doBullets(void);
static void drawPlayer(void);
static void drawBullets(void);

static Entity *player;
static SDL_Texture *bulletTexture;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();
    bulletTexture = loadTexture("gfx/playerBullet.png");
}

static void initPlayer(void)
{

}

static void logic(void)
{

}

static void doPlayer(void)
{

}

static void fireBullet(void)
{

}

static void doBullets(void)
{

}

static void draw(void)
{

}

static void drawPlayer(void)
{

}

static void drawBullets(void)
{

}
