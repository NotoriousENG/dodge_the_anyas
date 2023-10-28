import pygame
from .scene import Scene
from .scene_manager import SceneManager


class TitleScene(Scene):
    def __init__(self):
        self.flicker_time = 0.75
        self.current_time = 0.0
        self.draw_start = True
        self.countdown = 10

    def Update(self, deltaTime: float):
        self.current_time += deltaTime
        if self.current_time >= self.flicker_time:
            self.current_time = 0.0
            self.draw_start = not self.draw_start

        self.countdown -= deltaTime

        # if space is pressed, switch to main scene
        keys = pygame.key.get_pressed()
        if keys[pygame.K_SPACE]:
            from .main_scene import MainScene
            SceneManager.LoadScene(MainScene())
        elif self.countdown <= 0:
            from .high_score_scene import HighScoreScene
            SceneManager.LoadScene(HighScoreScene())

    def Draw(self, windowSurface: pygame.Surface):
        # draw the title
        font = pygame.font.SysFont('Arial', 60)
        text = font.render('Dodge the Anyas!', True, (255, 255, 255))
        textRect = text.get_rect()
        textRect.center = (400, 300)
        windowSurface.blit(text, textRect)

        if self.draw_start:
            # draw press space to start
            font = pygame.font.SysFont('Arial', 30)
            text = font.render('Press Space to Start', True, (255, 255, 255))
            textRect = text.get_rect()
            textRect.center = (400, 500)
            windowSurface.blit(text, textRect)
