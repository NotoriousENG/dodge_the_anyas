class SceneManager:
    @staticmethod
    def LoadScene(scene):
        SceneManager.current_scene = scene
        return SceneManager.current_scene
