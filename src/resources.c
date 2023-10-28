#include "resources.h"
#include "title_scene.h"

Resources load_resources(SDL_Renderer *renderer) {
  Resources resources;
  memset(&resources, 0, sizeof(Resources));
  resources.font_small = loadFont("assets/fonts/cat_paw.ttf", FONT_SIZE_SMALL);
  resources.font_large = loadFont("assets/fonts/cat_paw.ttf", FONT_SIZE_LARGE);
  resources.player_texture = loadTexture(renderer, "assets/sprites/tink.png");
  resources.enemy_texture = loadTexture(renderer, "assets/sprites/anya.png");
  resources.bg_texture = loadTexture(renderer, "assets/sprites/bg.png");
  resources.music = load_music("assets/music/happy.ogg");
  return resources;
}

void load_scene(Resources *resources, void *scene,
                void (*scene_draw)(void *scene, SDL_Renderer *renderer),
                void (*scene_logic)(void *scene, float delta)) {
  resources->scene = scene;
  resources->scene_draw = scene_draw;
  resources->scene_logic = scene_logic;
}

void load_scene_by_name(Resources *resources, char *name) {
  if (strcmp(name, TITLE_SCENE) == 0) {
    load_scene(resources, resources->title_scene, (void *)title_draw,
               (void *)title_logic);
  } else if (strcmp(name, GAME_SCENE) == 0) {
    load_scene(resources, resources->game_scene, (void *)game_draw,
               (void *)game_logic);
  } else if (strcmp(name, LEADERBOARD_SCENE) == 0) {
    load_scene(resources, resources->leaderboard_scene,
               (void *)leaderboard_draw, (void *)leaderboard_update);
  } else {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Failed to load scene: %s", name);
    return;
  }
}

void free_resources(Resources resources) {
  // free the loaded scenes
  title_free(resources.title_scene);
  game_free(resources.game_scene);
  leaderboard_free(resources.leaderboard_scene);
  // free the assets
  Mix_FreeMusic(resources.music);
  TTF_CloseFont(resources.font_small);
  TTF_CloseFont(resources.font_large);
  SDL_DestroyTexture(resources.player_texture);
  SDL_DestroyTexture(resources.enemy_texture);
  SDL_DestroyTexture(resources.bg_texture);
}

Mix_Music *load_music(char *filename) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);

  Mix_Music *music = Mix_LoadMUS(filename);

  if (music == NULL) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Failed to load %s", filename);
    exit(1);
  }
  return music;
}

void play_music_on_loop(Mix_Music *music) {
  if (Mix_PlayingMusic() == 0) {
    Mix_PlayMusic(music, -1);
  }
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, char *filename) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);

  if (renderer == NULL) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Failed to load %s RENDERER IS NULL", filename);
    exit(1);
  }

  if (texture == NULL) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Failed to load %s", filename);
    exit(1);
  }
  return texture;
}

void draw(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
          SDL_RendererFlip flip) {
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

  SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0.0, NULL, flip);
}

TTF_Font *loadFont(char *filename, int size) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                 "Loading %s", filename);
  TTF_Font *font = TTF_OpenFont(filename, size);
  if (font == NULL) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Failed to load font: %s", TTF_GetError());
    exit(1);
  }
  return font;
}

void drawText(SDL_Renderer *renderer, char *text, int x, int y, TTF_Font *font,
              SDL_Color color) {
  int wrapLength = SCREEN_WIDTH;
  SDL_Surface *surface =
      TTF_RenderText_Solid_Wrapped(font, text, color, wrapLength);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
  SDL_RenderCopy(renderer, texture, NULL, &dest);
  SDL_DestroyTexture(texture);
}
