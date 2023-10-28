#include "leaderboard_scene.h"
#include "resources.h"
#include <sinput.h>

LeaderboardScene *leaderboard_new(Resources *resources) {
  LeaderboardScene *scene =
      (LeaderboardScene *)malloc(sizeof(LeaderboardScene));
  scene->resources = resources;

  if (!read_highscores(scene->records, NUM_HIGHSCORES)) {
    for (int i = 0; i < NUM_HIGHSCORES; i++) {
      scene->records[i] = highscore_new("Anonymous", 0);
    }
  }

  scene->resources->last_score = 0;
  // initialize the name to empty
  memset(scene->name, '\0', HS_NAME_LENGTH + 1);

  scene->added_index = -1;

  return scene;
}

void leaderboard_free(LeaderboardScene *scene) { free(scene); }

void leaderboard_draw(LeaderboardScene *scene, SDL_Renderer *renderer) {
  int top = 50;
  int left = 300;
  SDL_Color white = {255, 255, 255, 255};
  SDL_Color selected = {0, 255, 255, 255};
  drawText(renderer, "Highscores:", left, top, scene->resources->font_large,
           white);

  left += 30;
  top += 75;

  for (int i = 0; i < NUM_HIGHSCORES; i++) {
    SDL_Color *color = &white;
    if (i == scene->added_index) {
      color = &selected;
    }
    char score_string[64];
    sprintf(score_string, "%i. %s - %i", i + 1, scene->records[i].name,
            scene->records[i].score);
    top += 50;
    drawText(renderer, score_string, left, top, scene->resources->font_small,
             *color);
  }

  left += 20;
  top += 100;
  // draw your score
  char score_string[64];
  sprintf(score_string, "Your score: %i", scene->resources->last_score);
  drawText(renderer, score_string, left, top, scene->resources->font_small,
           white);

  if (!scene->added) {
    left = 100;
    top += 50;
    drawText(renderer, "Enter your name:", left, top,
             scene->resources->font_small, white);

    left += 300;

    drawText(renderer, scene->name, left, top, scene->resources->font_small,
             white);
    return;
  }

  if (scene->draw_start) {
    drawText(renderer, "Press space to play", 310, 550,
             scene->resources->font_small, white);
  }
}

void leaderboard_update(LeaderboardScene *scene, float delta) {

  if (!scene->added) {
    leaderboard_enter_name(scene);
    return;
  }

  scene->current_time += delta;

  if (scene->current_time > 1.0f) {
    scene->draw_start = !scene->draw_start;
    scene->current_time = 0.0f;
  }

  scene->toggle_timer += delta;
  if (scene->toggle_timer > SCENE_TOGGLE_TIME) {
    scene->toggle_timer = 0.0f;
    scene->resources->title_scene->toggle_timer = 0.0f;
    return load_scene_by_name(scene->resources, TITLE_SCENE);
  }

  if (scene->resources->keyboard[SDL_SCANCODE_SPACE] == JUST_PRESSED) {
    load_scene_by_name(scene->resources, GAME_SCENE);
  }
}

void leaderboard_enter_name(LeaderboardScene *scene) {
  // use text input to enter name
  SDL_StartTextInput();
  int i, n;
  char c;

  n = strlen(scene->name);

  for (i = 0; i < strlen(scene->resources->input_text); i++) {
    c = (scene->resources->input_text[i]);

    if (n < HS_NAME_LENGTH) {
      scene->name[n++] = c;
    }
  }

  if (n > 0 &&
      scene->resources->keyboard[SDL_SCANCODE_BACKSPACE] == JUST_PRESSED) {
    scene->name[--n] = '\0';
  }

  // entered name
  if (scene->resources->keyboard[SDL_SCANCODE_RETURN] == JUST_PRESSED) {
    if (strlen(scene->name) == 0) {
      STRNCPY(scene->name, "Anonymous", HS_NAME_LENGTH + 1);
    }

    SDL_StopTextInput();

    scene->added_index = leaderboard_add_record(
        scene->records, NUM_HIGHSCORES,
        highscore_new(scene->name, scene->resources->last_score));

    // clear the name
    memset(scene->name, '\0', HS_NAME_LENGTH + 1);
    scene->added = 1;
  }

  // clear the input text
  memset(scene->resources->input_text, '\0', HS_NAME_LENGTH + 1);
}

int leaderboard_add_record(HighscoreRecord *records, int length,
                           HighscoreRecord new) {
  // find the index to insert the new record
  int index = -1;
  for (int i = 0; i < length; i++) {
    if (new.score >= records[i].score) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return index;
  }

  // shift the records down
  for (int i = length - 1; i > index; i--) {
    records[i] = records[i - 1];
  }

  // insert the new record
  records[index] = new;

  // write the records to file
  write_highscores(records, length);

  return index;
}

int read_highscores(HighscoreRecord *records, int length) {
  FILE *file = fopen("assets/highscores.bin", "rb");
  if (file == NULL) {
    return 0;
  }

  fread(records, sizeof(HighscoreRecord), length, file);
  fclose(file);
  return 1;
}

void write_highscores(HighscoreRecord *records, int length) {
  FILE *file = fopen("assets/highscores.bin", "wb");
  if (file == NULL) {
    return;
  }

  fwrite(records, sizeof(HighscoreRecord), length, file);
  fclose(file);
}
