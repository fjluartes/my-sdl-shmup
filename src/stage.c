/*
 * stage.c: file that handles actual game logic
 * Revision: 2026-06-19 fjluartes
 */
#include "common.h"

#include "draw.h"
#include "stage.h"
#include "util.h"

extern App app;
extern Stage stage;

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);
static void drawFighters(void);
static void drawBullets(void);
static void spawnEnemies(void);
static int bulletHitFighter(Entity *b);
static void doEnemies(void);
static void fireAlienBullet(Entity *e);
static void clipPlayer(void);
static void resetStage(void);
static void drawBackground(void);
static void doBackground(void);
static void drawDebris(void);

static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *alienBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *background;
static SDL_Texture *explosionTexture;
static int enemySpawnTimer;
static int stageResetTimer;
static int backgroundX;
// static Star stars[MAX_STARS];

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    bulletTexture = loadTexture("gfx/playerBullet.png");
    enemyTexture = loadTexture("gfx/enemy.png");
    alienBulletTexture = loadTexture("gfx/alienBullet.png");
    playerTexture = loadTexture("gfx/player.png");
    background = loadTexture("gfx/background.png");
    explosionTexture = loadTexture("gfx/explosion.png");

    resetStage();
}

static void resetStage(void)
{
    Entity *e;
    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    enemySpawnTimer = 0;
    stageResetTimer = FPS * 2;
}

static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->health = 3;
    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;
}

static void logic(void)
{
    doBackground();
    if (player != NULL) doPlayer();
    doEnemies();
    doFighters();
    doBullets();
    spawnEnemies();
    clipPlayer();
    if (player == NULL && --stageResetTimer <= 0) 
        resetStage();
}

static void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

static void doPlayer(void)
{
    player->dx = player->dy = 0;
    if (player->reload > 0) player->reload--;
    if (app.keyboard[SDL_SCANCODE_UP]) player->dy = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_DOWN]) player->dy = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_LEFT]) player->dx = -PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_RIGHT]) player->dx = PLAYER_SPEED;
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0)
        fireBullet();
}

static void fireBullet(void)
{
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    bullet->side = SIDE_PLAYER;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);
    player->reload = 8;
}

static void doEnemies(void)
{
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
            fireAlienBullet(e);
    }
}

static void fireAlienBullet(Entity *e)
{
    Entity *bullet;
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2),
                           e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;
    e->reload = (rand() % FPS * 2);
}

static void doFighters(void)
{
    Entity *e, *prev;
    prev = &stage.fighterHead;
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x < -e->w) e->health = 0;
        if (e->health == 0)
        {
            if (e == player) player = NULL;
            if (e == stage.fighterTail) stage.fighterTail = prev;
            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}

static void doBullets(void)
{
    Entity *b, *prev;
    prev = &stage.bulletHead;
    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;
        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h ||
            b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
        {
            if (b == stage.bulletTail) stage.bulletTail = prev;
            prev->next = b->next;
            free(b);
            b = prev;
        }
        prev = b;
    }
}

static int bulletHitFighter(Entity *b)
{
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side && 
            collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            if (e == player && e->health > 0)
            {
                b->health = 0;
                e->health -= 1;
            }
            else 
            {
                b->health = 0;
                e->health = 0;
            }
            return 1;
        }
    }
    return 0;
}

static void spawnEnemies(void)
{
    Entity *enemy;
    if (--enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));

        enemy->side = SIDE_ALIEN;
        enemy->health = 1;

        enemy->reload = FPS * (1 + (rand() % 3));
        enemySpawnTimer = 30 + (rand() % 60);
    }
}

static void clipPlayer(void)
{
    if (player != NULL) 
    {
        if (player->x < 0) player->x = 0;
        if (player->y < 0) player->y = 0;
        if (player->x > SCREEN_WIDTH / 2) player->x = SCREEN_WIDTH / 2;
        if (player->y > SCREEN_HEIGHT) player->y = SCREEN_HEIGHT;
    }
}

static void draw(void)
{
    drawBackground();
    drawFighters();
    drawDebris();
    drawBullets();
}

static void drawFighters(void)
{
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y);    
    }
}

static void drawBullets(void)
{
    Entity *b;
    for (b = stage.bulletHead.next; b!=NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}

static void drawBackground(void)
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

static void drawDebris(void)
{
    Debris *d;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}
