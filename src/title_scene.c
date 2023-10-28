#include "title_scene.h"
#include "defs.h"
#include "resources.h"
#include <sinput.h>

TitleScene *title_new(Resources *resources) {
  TitleScene *scene = (TitleScene *)malloc(sizeof(TitleScene));
  scene->resources = resources;
  scene->current_time = 0;
  scene->draw_start = 0;
  return scene;
}

void title_free(TitleScene *scene) { free(scene); }

void title_draw(TitleScene *scene, SDL_Renderer *renderer) {
  drawText(renderer, "Dodge the Anyas!", 200, 200, scene->resources->font_large,
           (SDL_Color){255, 255, 255, 255});

  if (scene->draw_start) {
    drawText(renderer, "Press space to start", 260, 400,
             scene->resources->font_small, (SDL_Color){255, 255, 255, 255});
  }
}

void title_logic(TitleScene *scene, float delta) {
  scene->current_time += delta;

  if (scene->current_time > FLICKER_TIME) {
    scene->draw_start = !scene->draw_start;
    scene->current_time = 0;
  }

  if (scene->resources->keyboard[SDL_SCANCODE_SPACE] == JUST_PRESSED) {
    load_scene_by_name(scene->resources, GAME_SCENE);
  }
}