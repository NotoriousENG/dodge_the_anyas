from random import randint
import pygame
from .game_object import GameObject


class Enemy(GameObject):
    def __init__(self, position: pygame.Vector2, texture: pygame.image, collision_rect: pygame.Rect):
        super().__init__(position, texture, collision_rect)
        self.speed = 200

    def Update(self, deltaTime: float):
        self.position.y += self.speed * deltaTime
        if self.position.y > 600:
            self.position.y = -50
            self.position.x = randint(0, 800)
