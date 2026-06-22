/*
 * highscores.c: File to save and show highscores
 */
#include "common.h"

#include "background.h"
#include "highscores.h"
#include "stage.h"
#include "text.h"

extern App app;
extern Highscores highscores;

static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);

void initHighscoreTable(void)
{
    int i;
    memset(&highscores, 0, sizeof(Highscores));
    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.highscore[i].score = NUM_HIGHSCORES - i;
    }
}

void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
}

static void logic(void)
{
    doBackground();

    doStarfield();

    if (app.keyboard[SDL_SCANCODE_SPACE])
        initStage();
}

static void draw(void)
{
    drawBackground();

    drawStarfield();

    drawHighscores();
}

static void drawHighscores(void)
{
    int i, y;
    y = 150;

    drawText(425, 70, 255, 255, 255, "HIGHSCORES");

    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        if (highscores.highscore[i].recent)
        {
            drawText(425, y, 255, 255, 0, "#%d ............. %03d", 
                (i + 1), highscores.highscore[i].score);
        }
        else 
        {
            drawText(425, y, 255, 255, 255, "#%d ............. %03d", 
                (i + 1), highscores.highscore[i].score);
        }

        y += 50;
    }
    drawText(425, 600, 255, 255, 255, "PRESS FIRE TO PLAY!");
}

void addHighscore(int score)
{
    Highscore newHighscores[NUM_HIGHSCORES + 1];
    int i;

    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        newHighscores[i] = highscores.highscore[i];
        newHighscores[i].recent = 0;
    }

    newHighscores[NUM_HIGHSCORES].score = score;
    newHighscores[NUM_HIGHSCORES].recent = 1;

    qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

    for (i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.highscore[i] = newHighscores[i];
    }
}

static int highscoreComparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore *)a);
    Highscore *h2 = ((Highscore *)b);

    return h2->score - h1->score;
}