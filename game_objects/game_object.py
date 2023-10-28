import pygame


class GameObject:
    def __init__(self, position: pygame.Vector2, texture: pygame.image, collision_rect: pygame.Rect):
        self.position = position
        self.texture = texture
        self.origin_collision_rect = collision_rect
        self.UpdateCollider()

    def Update(self, deltaTime: float):
        pass

    def UpdateCollider(self):
        self.collision_rect = self.origin_collision_rect.move(self.position)

    def Draw(self, windowSurface: pygame.Surface):
        windowSurface.blit(self.texture, self.position)

    def DrawCollider(self, windowSurface: pygame.Surface):
        pygame.draw.rect(windowSurface, (255, 0, 0), self.collision_rect, 1)

    def Overlaps(self, other: 'GameObject'):
        # chack if the rectangles overlap for self and other (position + texture size)
        return self.collision_rect.colliderect(other.collision_rect)
