#include "entity.h"
#include "resources.h"

Entity entity_new(SDL_Texture *texture, vec2 position, SDL_Rect collider,
                  int enabled) {
  Entity entity;
  memset(&entity, 0, sizeof(Entity));
  entity.texture = texture;
  entity.position = position;
  entity.collider = collider;
  entity.enabled = enabled;

  return entity;
}

void entity_draw(Entity *entity, SDL_Renderer *renderer,
                 SDL_RendererFlip flip) {
  if (!entity->enabled) {
    return;
  }

  draw(renderer, entity->texture, entity->position.x, entity->position.y, flip);
}

void entity_draw_collider(Entity *entity, SDL_Renderer *renderer) {
  if (!entity->enabled) {
    return;
  }
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect transformed_collider = entity->collider;
  transformed_collider.x += entity->position.x;
  transformed_collider.y += entity->position.y;
  SDL_RenderDrawRect(renderer, &transformed_collider);
}

int is_colliding(Entity *entity, Entity *other) {
  if (!entity->enabled || !other->enabled) {
    return 0;
  }
  SDL_Rect transformed_entity_collider = entity->collider;
  transformed_entity_collider.x += entity->position.x;
  transformed_entity_collider.y += entity->position.y;

  SDL_Rect transformed_other_collider = other->collider;
  transformed_other_collider.x += other->position.x;
  transformed_other_collider.y += other->position.y;

  if (SDL_HasIntersection(&transformed_entity_collider,
                          &transformed_other_collider)) {
    return 1;
  }
  return 0;
}