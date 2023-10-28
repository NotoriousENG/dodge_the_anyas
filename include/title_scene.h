#pragma once

#include "defs.h"
#include <SDL.h>

typedef struct Resources Resources;

typedef struct TitleScene {
  Resources *resources;
  float current_time;
  int draw_start;
} TitleScene;

TitleScene *title_new(Resources *resources);

void title_free(TitleScene *scene);

void title_draw(TitleScene *scene, SDL_Renderer *renderer);

void title_logic(TitleScene *scene, float delta);