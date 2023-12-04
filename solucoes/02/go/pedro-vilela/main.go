package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	file, err := os.Open("input1.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	partOne(lines)
	partTwo(lines)
}

func partOne(lines []string) {
	var possibleGames int
	maxCubes := map[string]int{
		"red":   12,
		"green": 13,
		"blue":  14,
	}

outer:
	for i, line := range lines {
		line = strings.Replace(line, ",", "", -1)
		line = strings.Replace(line, ";", "", -1)
		words := strings.Fields(line) // split on whitespace
		for w, word := range words {
			if number, err := strconv.Atoi(word); err == nil { // check if word is a number
				cubeColor := words[w+1]           // check the word after each number to get the cube color
				if number > maxCubes[cubeColor] { // check if the number is greater than the max cubes for that color
					continue outer // if it is, game is impossible, continue to next line
				}
			}
		}
		possibleGames += i + 1 // if we get here, the game is possible, add 1 to the possible games
	}
	log.Println("Part one:", possibleGames)
}

func partTwo(lines []string) {
	var powerSum int
	minCubes := map[string]int{
		"red":   0,
		"green": 0,
		"blue":  0,
	}

	for _, line := range lines {
		line = strings.Replace(line, ",", "", -1)
		line = strings.Replace(line, ";", "", -1)
		words := strings.Fields(line)
		for w, word := range words {
			if number, err := strconv.Atoi(word); err == nil {
				cubeColor := words[w+1]
				if number > minCubes[cubeColor] { // check if the number is greater than the current min cubes for that color
					minCubes[cubeColor] = number // if it is, update the min cubes for that color
				}
			}
		}
		powerSum += int(minCubes["red"] * minCubes["green"] * minCubes["blue"]) // add the product of the min cubes for each color to the sum
		minCubes = map[string]int{                                              // reset the min cubes for each color to 0
			"red":   0,
			"green": 0,
			"blue":  0,
		}
	} // repeat for each line
	log.Println("Part two:", powerSum)
}
