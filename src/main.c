#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include <sinput.h>

#include "defs.h"
#include "entity.h"

#include "resources.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Resources resources;
// static Scene scene;
static float currentTime = 0;
static float delta = 0;
static float clear_color[3] = {0, 0, 0};

void cleanup() {

  free_resources(resources);

  TTF_Quit();

  Mix_Quit();

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);

  IMG_Quit();

  SDL_Quit();
}

void main_loop() {
  float lastTime = SDL_GetTicks() / 1000.0f;
  float delta = lastTime - currentTime;
  currentTime = lastTime;

  SDL_SetRenderDrawColor(renderer, clear_color[0], clear_color[1],
                         clear_color[2], 255);

  SDL_RenderClear(renderer);

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_TEXTINPUT:
      STRNCPY(resources.input_text, event.text.text, HS_NAME_LENGTH + 1);
      break;
    }
  }

  doInput(resources.keyboard);

  resources.scene_logic(resources.scene, delta);

  // draw the background image
  SDL_Rect bg_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderCopy(renderer, resources.bg_texture, NULL, &bg_rect);

  // with a tranparent black overlay
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
  SDL_RenderFillRect(renderer, &bg_rect);

  resources.scene_draw(resources.scene, renderer);

  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  // Boilerplate code for SDL2

  int rendererFlags, windowFlags;

  // rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  // prevent screen tear
  rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                  SDL_RENDERER_TARGETTEXTURE;

  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  // frequency of 44100 (CD quality), the default format, 2 channels (stereo)
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    printf("Couldn't initialize SDL Mixer\n");
    exit(1);
  }

  if (TTF_Init() < 0) {
    printf("Couldn't initialize SDL TTF\n");
    exit(1);
  }

  Mix_AllocateChannels(MAX_SOUND_CHANNELS);

  window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, windowFlags);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  renderer = SDL_CreateRenderer(window, -1, rendererFlags);

  if (!renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
    printf("Couldn't initialize SDL Image\n");
    exit(1);
  }

  if (SDL_NumJoysticks() < 1) {
    printf("No joysticks connected\n");
  }
  // End of boilerplate code

  resources = load_resources(renderer);
  resources.title_scene = title_new(&resources);
  resources.game_scene = game_new(&resources);
  resources.leaderboard_scene = leaderboard_new(&resources);
  load_scene_by_name(&resources, TITLE_SCENE);

  play_music_on_loop(resources.music);

  atexit(cleanup);

  resources.running = 1;

  // set blend mode
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop, 0, resources.running);
#else
  while (resources.running) {
    main_loop();
  }
#endif

  return 0;
}