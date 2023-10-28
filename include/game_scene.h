#pragma once

#include "defs.h"
#include "entity.h"
#include <SDL.h>

typedef struct Resources Resources;

typedef struct GameScene {
  Resources *resources;
  float current_time;
  float score;
  Entity player;
  Entity enemies[MAX_ENEMIES];
  int draw_colliders;
  int tex_w;
  int tex_h;
  int enabled_enemies;
} GameScene;

GameScene *game_new(Resources *resources);

void game_free(GameScene *scene);

void game_draw(GameScene *scene, SDL_Renderer *renderer);

void game_logic(GameScene *scene, float delta);

void player_logic(GameScene *scene, float delta);

void enemy_logic(GameScene *scene, float delta);

void add_enemies(GameScene *scene, int count);

void game_over(GameScene *scene);

void game_begin(GameScene *scene);