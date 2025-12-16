package main

import (
	"bufio"
	"os"
	"strings"
)

func ParseSRTFile(subtitleFile string) ([]Subtitle, error) {
	var subs []Subtitle

	file, err := os.Open(subtitleFile)
	if err != nil {
		return subs, err
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.ReplaceAll(scanner.Text(), "\r", "")

		var sub Subtitle
		subs = append(subs, sub)
	}

	return subs, nil
}
