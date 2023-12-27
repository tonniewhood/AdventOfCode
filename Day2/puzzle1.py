
def games_is_valid(game):

    gameValid = True

    for hand in game:

        rolls = hand.split(",")

        redDieInRoll = 0
        greenDieInRoll = 0
        blueDieInRoll = 0

        for die in rolls:

            if die.find("red") != -1:
                value = die.replace("red", "")
                redDieInRoll = int(value)

            if die.find("green") != -1:
                value = die.replace("green", "")
                greenDieInRoll = int(value)

            if die.find("blue") != -1:
                value = die.replace("blue", "")
                blueDieInRoll = int(value)

        gameValid = gameValid and (redDieInRoll <= 12) and (greenDieInRoll <= 13) and (blueDieInRoll <= 14)

    return gameValid
    



def main():

    with open("./puzzleInput.txt", "r") as puzzle:

        validIds = []
        for line in puzzle.readlines():
            line = line.strip()
            
            firstSplit = line.split(":")

            naiveId = firstSplit[0]
            naiveGame = firstSplit[1]
            gameSplit = naiveGame.split(";")

            if games_is_valid(gameSplit):
                validIds.append(int(naiveId[5:]))

        print(sum(validIds))

if __name__ == '__main__':
    main()