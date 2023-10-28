#include "leaderboard_scene.h"
#include "resources.h"
#include <sinput.h>

LeaderboardScene *leaderboard_new(Resources *resources) {
  LeaderboardScene *scene =
      (LeaderboardScene *)malloc(sizeof(LeaderboardScene));
  scene->resources = resources;

  for (int i = 0; i < NUM_HIGHSCORES; i++) {
    scene->records[i] = highscore_new("Anonymous", 0);
  }

  scene->resources->last_score = 100;

  return scene;
}

void leaderboard_free(LeaderboardScene *scene) { free(scene); }

void leaderboard_draw(LeaderboardScene *scene, SDL_Renderer *renderer) {
  int top = 50;
  int left = 300;
  drawText(renderer, "Highscores:", left, top, scene->resources->font_large,
           (SDL_Color){255, 255, 255, 255});

  left += 30;
  top += 75;

  for (int i = 0; i < NUM_HIGHSCORES; i++) {
    char score_string[64];
    sprintf(score_string, "%i. %s - %i", i + 1, scene->records[i].name,
            scene->records[i].score);
    top += 50;
    drawText(renderer, score_string, left, top, scene->resources->font_small,
             (SDL_Color){255, 255, 255, 255});
  }

  left += 20;
  top += 100;
  // draw your score
  char score_string[64];
  sprintf(score_string, "Your score: %i", scene->resources->last_score);
  drawText(renderer, score_string, left, top, scene->resources->font_small,
           (SDL_Color){255, 255, 255, 255});

  if (scene->draw_start) {
    drawText(renderer, "Press space to play", 310, 550,
             scene->resources->font_small, (SDL_Color){255, 255, 255, 255});
  }
}

void leaderboard_update(LeaderboardScene *scene, float delta) {
  scene->current_time += delta;

  if (scene->current_time > 1.0f) {
    scene->draw_start = !scene->draw_start;
    scene->current_time = 0.0f;
  }

  if (scene->resources->keyboard[SDL_SCANCODE_SPACE] == JUST_PRESSED) {
    load_scene_by_name(scene->resources, GAME_SCENE);
  }
}
