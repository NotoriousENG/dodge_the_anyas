import pygame

from random import randint
from game_objects import Enemy, GameObject, Player
from records import Records
from resources import Resources
from .scene import Scene
from .scene_manager import SceneManager
from .high_score_scene import HighScoreScene


class MainScene(Scene):
    def __init__(self):
        self.gameObjects: [GameObject] = []
        self.score = 0
        self.start_time = pygame.time.get_ticks() // 1000

        self.common_collider = pygame.Rect(18, 6, 26, 49)

        self.gameObjects.append(Player(pygame.Vector2(
            400, 540), Resources.player_texture, self.common_collider))

        self.add_enemies()

        self.spawn_timer = 10.0

    def add_enemies(self):
        for i in range(0, 10):
            # ensure no overlaps
            position = pygame.Vector2(randint(0, 800), randint(-600, 0))
            self.gameObjects.append(
                Enemy(position, Resources.enemy_texture, self.common_collider))

    def Update(self, deltaTime: float):
        for go in self.gameObjects:
            go.Update(deltaTime)
            if isinstance(go, Player):
                for other in self.gameObjects:
                    if go != other and go.Overlaps(other):
                        Records.SetCurrentScore(self.score)
                        SceneManager.LoadScene((HighScoreScene()))
        t = pygame.time.get_ticks() // 1000
        self.score = t - self.start_time

        self.spawn_timer -= deltaTime
        if self.spawn_timer <= 0:
            self.spawn_timer = 10.0
            self.add_enemies()

    def Draw(self, windowSurface: pygame.Surface):
        # super draw
        super().Draw(windowSurface)

        # draw score in the top left
        font = pygame.font.SysFont('Arial', 30)
        text = font.render('Score: ' + str(self.score), True, (0, 0, 0))
        textRect = text.get_rect()
        textRect.topleft = (0, 0)
        windowSurface.blit(text, textRect)
