#!/usr/bin/env pypy
import pygame
from records import Records
from resources import Resources
from scenes import SceneManager, TitleScene, HighScoreScene


class Game:
    def __init__(self):
        pygame.init()
        self.windowSurface = pygame.display.set_mode((800, 600))
        pygame.display.set_caption('Dodge the Anyas!')
        self.is_playing = True
        Resources.Load()
        Records.LoadHighScores()

        SceneManager.LoadScene(TitleScene())
        self.current_time = pygame.time.get_ticks()

        Resources.music.play(-1)

    @staticmethod
    def GetTextInput():
        return Game.text_input

    def Run(self):
        while self.is_playing:
            self.Update()
            self.Draw()

    def Update(self):
        # check for events
        events = pygame.event.get()
        Resources.text_input.update(events)
        for event in events:
            if event.type == pygame.QUIT:
                self.is_playing = False

        # get delta time
        deltaTime = (pygame.time.get_ticks() - self.current_time) / 1000.0
        self.current_time = pygame.time.get_ticks()
        SceneManager.current_scene.Update(deltaTime)

    def Draw(self):
        # draw the background (stretch to fit)
        self.windowSurface.blit(Resources.bg, (0, 0, 800, 600))

        # draw a transparent black overlay
        overlay = pygame.Surface((800, 600))
        overlay.set_alpha(128)
        overlay.fill((0, 0, 0))
        self.windowSurface.blit(overlay, (0, 0))

        # draw the scene
        SceneManager.current_scene.Draw(self.windowSurface)

        # draw the window onto the screen
        pygame.display.update()


if __name__ == '__main__':
    game = Game()
    game.Run()
