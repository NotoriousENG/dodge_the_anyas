#pragma once

#include "defs.h"
#include "highscores.h"
#include <SDL.h>

typedef struct Resources Resources;

typedef struct LeaderboardScene {
  Resources *resources;
  HighscoreRecord records[NUM_HIGHSCORES];
  float current_time;
  int draw_start;
} LeaderboardScene;

LeaderboardScene *leaderboard_new(Resources *resources);

void leaderboard_free(LeaderboardScene *scene);

void leaderboard_draw(LeaderboardScene *scene, SDL_Renderer *renderer);

void leaderboard_update(LeaderboardScene *scene, float delta);