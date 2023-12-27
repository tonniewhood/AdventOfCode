package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strings"
)

// Remeber, you can do this!

func trim(s string) string {

	newS := ""
	for i := 0; i < len(s); i++ {
		if s[i] != ' ' {
			newS += string(s[i])
		}
	}

	return newS
}

func arrayContains(array [10]string, s string) bool {

	for i := 0; i < len(array); i++ {
		if array[i] == s {
			return true
		}
	}

	return false
}

func main() {

	const (
		NUMBER_OF_WINNERS = 10
		NUMBER_OF_MINE    = 28
	)

	luckyNumbers, err1 := os.Open("puzzleInput.txt")

	if err1 != nil {
		log.Fatal(err1)
	}

	defer luckyNumbers.Close()

	scanner := bufio.NewScanner(luckyNumbers)

	powerSum := 0

	for scanner.Scan() {
		line := scanner.Text()
		words := strings.Split(line, " ")

		winners := [NUMBER_OF_WINNERS]string{}
		mine := [NUMBER_OF_MINE]string{}

		for i := 0; i < len(words); i++ {
			if i > 3 && i < 14 {
				winners[i-4] = words[i]
			}
			if i > 14 && i < 42 {
				mine[i-14] = words[i]
			}
		}

		var numberOfMatches float64 = 0
		for i := 0; i < len(mine); i++ {

			mineContainsWinner := arrayContains(winners, trim(mine[i]))
			if mineContainsWinner {
				numberOfMatches++
			}

		}

		var power float64 = 0.0
		var base float64 = 2.0

		if numberOfMatches > 0 {
			power = math.Pow(base, numberOfMatches-1)
		}

		powerSum += int(power)

		fmt.Printf("Line: %s \t Number of matches: %d \n", line, int(numberOfMatches))

		fmt.Print("Winners: [")
		for i := 0; i < len(winners); i++ {
			fmt.Printf("%s, ", trim(winners[i]))
		}

		fmt.Print("] \t Mine: [")
		for i := 0; i < len(mine); i++ {
			fmt.Printf("%s, ", trim(mine[i]))
		}
		fmt.Print("] \n")
	}

	fmt.Printf("Power sum: %d \n", powerSum)

}
