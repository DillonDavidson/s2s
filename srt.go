package main

import (
	"bufio"
	"os"
	"strconv"
	"strings"
)

func ParseSRTFile(subtitleFile string) ([]Subtitle, error) {
	var subs []Subtitle

	file, err := os.Open(subtitleFile)
	if err != nil {
		return subs, err
	}

	defer file.Close()

	var part, index int = 0, 0
	var text, start, end, line string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line = strings.ReplaceAll(scanner.Text(), "\r", "")

		if len(line) != 0 {
			if part == 0 {
				index, err = strconv.Atoi(line)
				if err != nil {
					return subs, err
				}
				part++
				continue
			}

			before, after, found := strings.Cut(line, "-->")
			if found {
				start = strings.TrimSpace(before)
				start = strings.ReplaceAll(start, ",", ".")
				end = strings.TrimSpace(after)
				end = strings.ReplaceAll(end, ",", ".")
			} else {
				if len(text) != 0 {
					text += " "
				}

				text += line
			}

			part++
		}

		if len(line) == 0 {
			subs = append(subs, NewSubtitle(index, start, end, text))
			part = 0
			text = ""
		}
	}

	if part != 0 {
		subs = append(subs, NewSubtitle(index, start, end, text))
	}

	return subs, nil
}
