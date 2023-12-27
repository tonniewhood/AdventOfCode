
def main():

    with open("puzzleInput.txt") as f:
        lines = f.readlines()

        scratchCards = []

        for cardNumber in range(1, 203):
            scratchCards.append(1)

        # print(f"Scratch cards: {scratchCards}")

        for lineIdx in range(len(lines)):
            words = lines[lineIdx].split()

            winners = words[2:12]
            mine = words[13:]

            # winners = words[2:7]
            # mine = words[8:]

            # print (f"Winners: {winners} -- Mine: {mine}\n")

            numberOfMatches = 0
            for number in mine:
                if number in winners:
                    print(f"{number} is in {winners}")
                    numberOfMatches += 1

            # print(f"{lines[lineIdx]} -- Number of matches: {numberOfMatches}\n")

            cardNumberIncrease = scratchCards[lineIdx]
            for cardIdx in range(lineIdx + 1, lineIdx + numberOfMatches + 1):
                # print(f"Adding {cardNumberIncrease} to card {cardIdx + 1}")
                scratchCards[cardIdx] += cardNumberIncrease

            
        print(f"Scratch cards: {scratchCards}")
        print(f"The total number of cards is: {sum(scratchCards)}")



if __name__ == "__main__":
    main()