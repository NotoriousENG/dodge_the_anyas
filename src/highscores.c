#include "highscores.h"
#include <string.h>

HighscoreRecord highscore_new(char *name, int score) {
  HighscoreRecord record;
  strcpy(record.name, name);
  record.score = score;
  return record;
}
