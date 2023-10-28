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
  char name[HS_NAME_LENGTH + 1];
  int added;
  int added_index;
  float toggle_timer;
} LeaderboardScene;

LeaderboardScene *leaderboard_new(Resources *resources);

void leaderboard_free(LeaderboardScene *scene);

void leaderboard_draw(LeaderboardScene *scene, SDL_Renderer *renderer);

void leaderboard_update(LeaderboardScene *scene, float delta);

void leaderboard_enter_name(LeaderboardScene *scene);

int leaderboard_add_record(HighscoreRecord *records, int length,
                           HighscoreRecord new);

int read_highscores(HighscoreRecord *records, int length);

void write_highscores(HighscoreRecord *records, int length);