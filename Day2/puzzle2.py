

def find_power(gameSplit):

    power = 0

    redRolls = []
    greenRolls = []
    blueRolls = []

    for hand in gameSplit:
            
        rolls = hand.split(",")

        for die in rolls:

            if die.find("red") != -1:
                value = die.replace("red", "")
                redRolls.append(int(value))

            if die.find("green") != -1:
                value = die.replace("green", "")
                greenRolls.append(int(value))

            if die.find("blue") != -1:
                value = die.replace("blue", "")
                blueRolls.append(int(value))

    power = max(redRolls) * max(greenRolls) * max(blueRolls)
    return power


def main():

    with open("./puzzleInput.txt", "r") as puzzle:

        validIds = []
        powerSum = 0
        for line in puzzle.readlines():
            line = line.strip()
            
            naiveGame = line.split(":")[1]
            gameSplit = naiveGame.split(";")

            powerSum += find_power(gameSplit)

        print(powerSum)

if __name__ == '__main__':
    main()