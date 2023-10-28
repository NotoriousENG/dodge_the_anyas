import pygame
from .game_object import GameObject


class Player(GameObject):
    def __init__(self, position: pygame.Vector2, texture: pygame.image, collision_rect: pygame.Rect):
        super().__init__(position, texture, collision_rect)
        self.speed = 400

    def Update(self, deltaTime: float):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a] or keys[pygame.K_LEFT]:
            self.position.x -= self.speed * deltaTime
        if keys[pygame.K_d] or keys[pygame.K_RIGHT]:
            self.position.x += self.speed * deltaTime

        if self.position.x < 0:
            self.position.x = 0
        elif self.position.x > 800 - self.texture.get_width():
            self.position.x = 800 - self.texture.get_width()
