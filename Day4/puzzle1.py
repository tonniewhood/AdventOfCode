
def main():

    with open("puzzleInput.txt") as f:
        lines = f.readlines()

        powerSum = 0

        for line in lines:
            words = line.split()

            winners = words[2:12]
            mine = words[13:]

            numberOfMatches = 0
            for number in mine:
                if number in winners:
                    numberOfMatches += 1

            # print(f"{line} -- Number of matches: {numberOfMatches}")

            linePower = 0

            if numberOfMatches == 1:
                linePower = 1
            elif numberOfMatches > 1:
                linePower = 1
                for i in range(numberOfMatches - 1):
                    linePower = linePower * 2

            print(f"Line: {line} \tNumber of Matches: {numberOfMatches}\n")
            powerSum += linePower

        print("Power sum: ", powerSum)


if __name__ == "__main__":
    main()