import pygame
import pygame_textinput


class Resources:
    text_input: pygame_textinput.TextInputVisualizer = None

    @staticmethod
    def Load():
        Resources.player_texture = pygame.image.load('assets/sprites/tink.png')
        Resources.enemy_texture = pygame.image.load('assets/sprites/anya.png')
        # flip enemy texture vertically
        Resources.enemy_texture = pygame.transform.flip(
            Resources.enemy_texture, False, True)

        Resources.music = pygame.mixer.Sound('assets/music/happy.mp3')

        Resources.bg = pygame.image.load('assets/sprites/bg.png')

        Resources.text_input = pygame_textinput.TextInputVisualizer()
