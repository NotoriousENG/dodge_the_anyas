import pygame


class GameObject:
    def __init__(self, position: pygame.Vector2, texture: pygame.image, collision_rect: pygame.Rect):
        self.position = position
        self.texture = texture
        self.origin_collision_rect = collision_rect
        self.enabled = True
        self.UpdateCollider()

    def Update(self, deltaTime: float):
        pass

    def UpdateCollider(self):
        if not self.enabled:
            return
        self.collision_rect = self.origin_collision_rect.move(self.position)

    def Draw(self, windowSurface: pygame.Surface):
        if not self.enabled:
            return

        rounded_positon = (round(self.position.x), round(self.position.y))
        windowSurface.blit(self.texture, rounded_positon)

    def DrawCollider(self, windowSurface: pygame.Surface):
        if not self.enabled:
            return
        pygame.draw.rect(windowSurface, (255, 0, 0), self.collision_rect, 1)

    def Overlaps(self, other: 'GameObject'):
        if not self.enabled or not other.enabled:
            return False
        # chack if the rectangles overlap for self and other (position + texture size)
        return self.collision_rect.colliderect(other.collision_rect)
