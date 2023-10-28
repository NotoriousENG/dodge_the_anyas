#pragma once

#define STRNCPY(dest, src, n)                                                  \
  strncpy(dest, src, n);                                                       \
  dest[n - 1] = '\0'

#define MAX_LINE_LENGTH 1024

#define MAX_SOUND_CHANNELS 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_SPEED 400.0f

#define FONT_SIZE_SMALL 30
#define FONT_SIZE_LARGE 60

#define FLICKER_TIME 0.75f
#define SCENE_TOGGLE_TIME 10.0f

#define COLLISION_BOUNDS                                                       \
  (SDL_Rect) { 18, 6, 26, 49 }

#define MAX_ENEMIES 1000

#define HS_NAME_LENGTH 10
#define NUM_HIGHSCORES 5

#define TITLE_SCENE "title"
#define GAME_SCENE "game"
#define LEADERBOARD_SCENE "leaderboard"

#ifndef GAME_NAME
#define GAME_NAME "Dodge the  Anya's!"
#endif