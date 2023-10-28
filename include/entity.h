#pragma once

#include <SDL.h>
#include <clm.h>

typedef struct Entity {
  vec2 position;
  SDL_Texture *texture;
  SDL_Rect collider;
  int enabled;
} Entity;

Entity entity_new(SDL_Texture *texture, vec2 position, SDL_Rect collider,
                  int enabled);

void entity_draw(Entity *entity, SDL_Renderer *renderer, SDL_RendererFlip flip);

void entity_draw_collider(Entity *entity, SDL_Renderer *renderer);

int is_colliding(Entity *entity, Entity *other);