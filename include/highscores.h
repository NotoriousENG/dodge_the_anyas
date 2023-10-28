#pragma once

#include "defs.h"

typedef struct HighscoreRecord {
  char name[HS_NAME_LENGTH + 1];
  int score;
} HighscoreRecord;

HighscoreRecord highscore_new(char *name, int score);
