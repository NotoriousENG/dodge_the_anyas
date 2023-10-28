import pickle


class Records:
    filepath = "assets/highscores.bin"
    current_score = 0
    high_scores: list[(str, int)] = []

    @staticmethod
    def LoadHighScores():
        try:
            with open(Records.filepath, "rb") as file:
                Records.high_scores = pickle.load(file)
        except:
            Records.high_scores = [
                ("Anonymous", 0),
                ("Anonymous", 0),
                ("Anonymous", 0),
                ("Anonymous", 0),
                ("Anonymous", 0),
            ]

    @staticmethod
    def SaveHighScores():
        with open(Records.filepath, "wb") as file:
            pickle.dump(Records.high_scores, file)

    def IsAHighScore(score):
        # check if score is higher than any of the high scores
        for high_score in Records.high_scores:
            if score > high_score[1]:
                return True

    @staticmethod
    def AddHighScore(record):
        Records.high_scores.append(record)
        Records.high_scores.sort(key=lambda x: x[1], reverse=True)
        Records.high_scores.pop()
        Records.SaveHighScores()
        return Records.high_scores.index(record)

    @staticmethod
    def GetHighScores():
        return Records.high_scores

    @staticmethod
    def SetCurrentScore(score):
        Records.current_score = score

    @staticmethod
    def GetCurrentScore():
        return Records.current_score
