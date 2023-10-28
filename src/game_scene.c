#include "game_scene.h"
#include "entity.h"
#include "resources.h"
#include <sinput.h>

GameScene *game_new(Resources *resources) {
  GameScene *scene = (GameScene *)malloc(sizeof(GameScene));
  scene->resources = resources;
  scene->draw_colliders = 0;
  game_begin(scene);

  return scene;
}

void game_free(GameScene *scene) { free(scene); }

void game_draw(GameScene *scene, SDL_Renderer *renderer) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (scene->enemies[i].enabled) {
      entity_draw(&scene->enemies[i], renderer, SDL_FLIP_VERTICAL);
      if (scene->draw_colliders) {
        entity_draw_collider(&scene->enemies[i], renderer);
      }
    } else {
      break;
    }
  }

  entity_draw(&scene->player, renderer, SDL_FLIP_NONE);
  if (scene->draw_colliders) {
    entity_draw_collider(&scene->player, renderer);
  }

  // draw score
  const char *score_label = "Score: ";
  int score_floor = (int)floor(scene->score);
  int score_len = score_floor / 10;
  if (score_len == 0) {
    score_len = 1;
  }
  char *score_string = malloc(strlen(score_label) + score_len + 1);
  sprintf(score_string, "Score: %i", score_floor);
  drawText(renderer, score_string, 0, 0, scene->resources->font_small,
           (SDL_Color){255, 255, 255, 255});
  free(score_string);
}

void game_logic(GameScene *scene, float delta) {
  if (scene->resources->keyboard[SDL_SCANCODE_1] == JUST_PRESSED) {
    scene->draw_colliders = !scene->draw_colliders;
  }
  player_logic(scene, delta);
  enemy_logic(scene, delta);

  // update score
  scene->score += delta;
  // add enemy timer
  scene->current_time += delta;
  if (scene->current_time > 10) {
    scene->current_time = 0;
    add_enemies(scene, 10);
  }
}

void player_logic(GameScene *scene, float delta) {

  float x = 0;

  // player movement on xy, clamp to screen (A, Left Arrow, D, Right Arrow)
  if (scene->resources->keyboard[SDL_SCANCODE_A] == PRESSED ||
      scene->resources->keyboard[SDL_SCANCODE_LEFT] == PRESSED) {
    scene->player.position.x -= PLAYER_SPEED * delta;
  }

  if (scene->resources->keyboard[SDL_SCANCODE_D] == PRESSED ||
      scene->resources->keyboard[SDL_SCANCODE_RIGHT] == PRESSED) {
    scene->player.position.x += PLAYER_SPEED * delta;
  }

  if (scene->player.position.x < 0) {
    scene->player.position.x = 0;
  }

  if (scene->player.position.x > SCREEN_WIDTH - scene->tex_w) {
    scene->player.position.x = SCREEN_WIDTH - scene->tex_w;
  }

  // check if player collides with enemies
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (scene->enemies[i].enabled) {
      if (is_colliding(&scene->player, &scene->enemies[i])) {
        game_over(scene);
        return;
      }
    } else {
      break;
    }
  }
}

void enemy_logic(GameScene *scene, float delta) {
  // enemy movement on y, wrap around screen
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (scene->enemies[i].enabled) {
      scene->enemies[i].position.y += 100 * delta;
      if (scene->enemies[i].position.y > SCREEN_HEIGHT) {
        scene->enemies[i].position.y =
            (rand() % SCREEN_HEIGHT) - SCREEN_HEIGHT - scene->tex_h;
        scene->enemies[i].position.x = rand() % SCREEN_WIDTH;
      }
    } else {
      break;
    }
  }
}

void add_enemies(GameScene *scene, int count) {
  int new_enabled_enemies = scene->enabled_enemies + count;
  if (new_enabled_enemies > MAX_ENEMIES) {
    new_enabled_enemies = MAX_ENEMIES;
  }
  for (int i = scene->enabled_enemies; i < new_enabled_enemies; i++) {
    scene->enemies[i].enabled = 1;
  }
  scene->enabled_enemies = new_enabled_enemies;
}

void game_over(GameScene *scene) {
  int final_score = (int)floor(scene->score);
  scene->resources->last_score = final_score;
  game_begin(scene);
  if (final_score >=
      scene->resources->leaderboard_scene->records[NUM_HIGHSCORES - 1].score) {
    scene->resources->leaderboard_scene->added = 0;
  } else {
    scene->resources->leaderboard_scene->added = 1;
  }
  scene->resources->leaderboard_scene->added_index = -1;
  load_scene_by_name(scene->resources, LEADERBOARD_SCENE);
}

void game_begin(GameScene *scene) {
  scene->current_time = 0;
  scene->score = 0;
  scene->player = entity_new(scene->resources->player_texture,
                             vec2_new(400, 540), COLLISION_BOUNDS, 1);

  for (int i = 0; i < MAX_ENEMIES; i++) {
    scene->enemies[i] = entity_new(
        scene->resources->enemy_texture,
        vec2_new(rand() % SCREEN_WIDTH,
                 (rand() % SCREEN_HEIGHT) - SCREEN_HEIGHT - scene->tex_h),
        COLLISION_BOUNDS, 0);
  }

  scene->enabled_enemies = 10;

  // enable first 10 enemies
  for (int i = 0; i < scene->enabled_enemies; i++) {
    scene->enemies[i].enabled = 1;
  }

  SDL_QueryTexture(scene->player.texture, NULL, NULL, &scene->tex_w,
                   &scene->tex_h);
}
