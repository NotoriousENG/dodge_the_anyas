import pygame
from game_objects import GameObject


class Scene:
    def __init__(self, gameObjects: [GameObject] = []):
        self.gameObjects = gameObjects

    def Update(self, deltaTime: float):
        for go in self.gameObjects:
            go.Update(deltaTime)

    def Draw(self, windowSurface: pygame.Surface):
        for go in self.gameObjects:
            go.Draw(windowSurface)
            go.UpdateCollider()
            caps = pygame.key.get_mods() & pygame.KMOD_CAPS
            if caps:
                go.DrawCollider(windowSurface)
