package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"unicode"
)

func main() {
	file, err := os.Open("./input1.txt")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	lines := []string{}
	for scanner.Scan() {
		line := scanner.Text()
		trimmedLine := strings.TrimSpace(line)
		lines = append(lines, trimmedLine)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println(err)
	}

	partOne(lines)
	partTwo(lines)
}

func partOne(lines []string) {
	numbers := []string{}
	for _, w := range lines { // iterate over all lines
		var firstNumber rune
		var lastNumber rune
		for _, r := range w { // iterate over all runes in a line
			if unicode.IsDigit(r) { // check if rune is a digit
				firstNumber = r // if it is, set it as the first number
				break
			}
		}
		for i := len(w) - 1; i >= 0; i-- { // iterate over all runes in a line in reverse
			if unicode.IsDigit(rune(w[i])) { // check if rune is a digit
				lastNumber = rune(w[i]) // if it is, set it as the last number
				break
			}
		}
		number := string(firstNumber) + string(lastNumber) // concatenate first and last number
		numbers = append(numbers, number)                  // add number to numbers slice
	}

	total := 0
	for _, n := range numbers { // iterate over all numbers
		num, err := strconv.Atoi(n) // convert string to int
		if err != nil {
			fmt.Println(err)
			return
		}
		total += num // add number to total
	}
	fmt.Println(total)
}

func partTwo(lines []string) {
	spelledOutNumbers := map[string]rune{
		"one":   '1',
		"two":   '2',
		"three": '3',
		"four":  '4',
		"five":  '5',
		"six":   '6',
		"seven": '7',
		"eight": '8',
		"nine":  '9',
	}
	numbers := []string{}
	for _, w := range lines { // iterate over all lines
		var firstNumber rune
		var lastNumber rune
	firstOuter:
		for i, r := range w { // iterate over all runes in a line
			if unicode.IsDigit(r) { // check if rune is a digit
				firstNumber = r // if it is, set it as the first number
				break firstOuter
			}
			for key, value := range spelledOutNumbers {
				if strings.HasPrefix(w[i:], key) { // check if substring starts with a spelled out number
					firstNumber = value // if it does, set it as the first number
					break firstOuter
				}
			}
		}
	lastOuter:
		for i := len(w) - 1; i >= 0; i-- { // iterate over all runes in a line in reverse
			if unicode.IsDigit(rune(w[i])) { // check if rune is a digit
				lastNumber = rune(w[i]) // if it is, set it as the last number
				break lastOuter
			}
			for key, value := range spelledOutNumbers {
				if strings.HasSuffix(w[:i+1], key) { // check if substring ends with a spelled out number
					lastNumber = value // if it does, set it as the first number
					break lastOuter
				}
			}
		}
		number := string(firstNumber) + string(lastNumber) // concatenate first and last number
		numbers = append(numbers, number)                  // add number to numbers slice
	}
	total := 0
	for _, n := range numbers { // iterate over all numbers
		num, err := strconv.Atoi(n) // convert string to int
		if err != nil {
			fmt.Println(err)
			return
		}
		total += num // add number to total
	}
	fmt.Println(total)
}
