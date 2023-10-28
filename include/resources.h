#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <sinput.h>

#include "defs.h"

#include "game_scene.h"
#include "leaderboard_scene.h"
#include "title_scene.h"
#include <SDL.h>

typedef struct Resources {
  Mix_Music *music;
  TTF_Font *font_small;
  TTF_Font *font_large;
  SDL_Texture *player_texture;
  SDL_Texture *enemy_texture;
  SDL_Texture *bg_texture;
  KeyState keyboard[MAX_KEYBOARD_KEYS];
  int running;
  int last_score;
  // Loaded Scenes
  TitleScene *title_scene;
  GameScene *game_scene;
  LeaderboardScene *leaderboard_scene;
  char input_text[HS_NAME_LENGTH + 1];
  // Current Scene
  void *scene;
  void (*scene_draw)(void *scene, SDL_Renderer *renderer);
  void (*scene_logic)(void *scene, float delta);
} Resources;

void load_scene(Resources *resources, void *scene,
                void (*scene_draw)(void *scene, SDL_Renderer *renderer),
                void (*scene_logic)(void *scene, float delta));

void load_scene_by_name(Resources *resources, char *name);

Resources load_resources(SDL_Renderer *renderer);

void free_resources(Resources resources);

Mix_Music *load_music(char *filename);

void play_music_on_loop(Mix_Music *music);

SDL_Texture *loadTexture(SDL_Renderer *renderer, char *filename);

// blit with rotation
void draw(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
          SDL_RendererFlip flip);

TTF_Font *loadFont(char *filename, int size);

// draw text with SDL_ttf with specified font
void drawText(SDL_Renderer *renderer, char *text, int x, int y, TTF_Font *font,
              SDL_Color color);
