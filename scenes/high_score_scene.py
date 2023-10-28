import pygame
from game_objects import GameObject
from records import Records
from resources import Resources
from .scene import Scene
from .scene_manager import SceneManager
from .title_scene import TitleScene


class HighScoreScene(Scene):
    def __init__(self, gameObjects: [GameObject] = []):
        self.game_objects = []
        self.high_scores = Records.GetHighScores()
        self.current_score = 0
        score = Records.GetCurrentScore()
        if Records.IsAHighScore(score):
            self.current_score = score
        Resources.text_input.value = ''
        Resources.text_input.cursor_color = (255, 255, 255)
        Resources.text_input.font_color = (255, 255, 255)
        self.countdown = 10
        self.draw_start = True
        self.flicker_time = 0.75
        self.current_time = 0.0
        self.highlighted = -1

    def Update(self, deltaTime: float):
        pressed = pygame.key.get_pressed()
        if self.current_score > 0:
            Resources.text_input.value = Resources.text_input.value[:10]
            # if enter is pressed, add the score to the high scores
            if pressed[pygame.K_RETURN] and len(Resources.text_input.value) > 0:
                self.highlighted = Records.AddHighScore(
                    (Resources.text_input.value, self.current_score))
                Records.SaveHighScores()
                self.current_score = 0
            return

        self.current_time += deltaTime
        if self.current_time >= self.flicker_time:
            self.current_time = 0.0
            self.draw_start = not self.draw_start

        self.countdown -= deltaTime
        # if space is pressed go to title scene
        if pressed[pygame.K_SPACE] or self.countdown <= 0:
            SceneManager.LoadScene(TitleScene())

    def Draw(self, windowSurface: pygame.Surface):
        # draw the high scores
        font = pygame.font.SysFont('Arial', 60)
        text = font.render('High Scores', True, (255, 255, 255))
        textRect = text.get_rect()
        textRect.center = (400, 100)
        windowSurface.blit(text, textRect)

        font = pygame.font.SysFont('Arial', 30)
        white = (255, 255, 255)
        red = (255, 0, 0)
        for i in range(len(self.high_scores)):
            color = white
            if self.highlighted == i:
                color = red
            text = font.render(
                str(self.high_scores[i][0] + ':'), True, color)
            windowSurface.blit(text, (280, 180 + i * 50))
            text = font.render(
                str(self.high_scores[i][1]), True, color)
            windowSurface.blit(text, (500, 180 + i * 50))

        # draw the current score
        if self.current_score > 0:
            font = pygame.font.SysFont('Arial', 30)
            text = font.render('Enter Your Name:', True, (255, 255, 255))
            textRect = text.get_rect()
            textRect.topright = (400, 450)
            windowSurface.blit(text, textRect)
            Resources.text_input.font_object = pygame.font.SysFont(
                'Arial', 30)
            windowSurface.blit(Resources.text_input.surface,
                               (textRect.x + textRect.w + 50, textRect.y))
            return

        if self.draw_start:
            # draw press space to start
            font = pygame.font.SysFont('Arial', 30)
            text = font.render('Press Space to Start', True, (255, 255, 255))
            textRect = text.get_rect()
            textRect.center = (400, 500)
            windowSurface.blit(text, textRect)
