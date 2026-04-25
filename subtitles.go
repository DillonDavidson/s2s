package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
)

func ParseSubtitleFile(subtitleFile string) ([]Subtitle, error) {
	extension := filepath.Ext(subtitleFile)

	switch extension {
	case ".ass":
		return ParseASSFile(subtitleFile)
	case ".srt":
		return ParseSRTFile(subtitleFile)
	default:
		return nil, fmt.Errorf("unsupported subtitle file extension")
	}
}

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
				line = strings.TrimPrefix(line, "\uFEFF")

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

func ParseASSFile(subtitleFile string) ([]Subtitle, error) {
	var subs []Subtitle

	file, err := os.Open(subtitleFile)
	if err != nil {
		return subs, err
	}
	defer file.Close()

	// Column indices parsed from the Format: line
	var startIdx, endIdx, textIdx int = -1, -1, -1
	inEvents := false

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.ReplaceAll(scanner.Text(), "\r", "")
		line = strings.TrimPrefix(line, "\uFEFF")

		// Track which section we're in
		if strings.HasPrefix(line, "[") {
			inEvents = strings.EqualFold(line, "[Events]")
			continue
		}

		if !inEvents {
			continue
		}

		// Parse the Format: line to learn column order
		if strings.HasPrefix(line, "Format:") {
			fields := strings.SplitN(line, ":", 2)
			if len(fields) < 2 {
				continue
			}
			cols := strings.Split(fields[1], ",")
			for i, col := range cols {
				switch strings.TrimSpace(col) {
				case "Start":
					startIdx = i
				case "End":
					endIdx = i
				case "Text":
					textIdx = i
				}
			}
			continue
		}

		// Parse Dialogue: lines
		if !strings.HasPrefix(line, "Dialogue:") {
			continue
		}
		if startIdx < 0 || endIdx < 0 || textIdx < 0 {
			return subs, fmt.Errorf("dialogue line found before Format: header")
		}

		// Strip the "Dialogue:" prefix, then split only up to textIdx+1
		// fields — this preserves commas inside the Text field.
		raw := strings.SplitN(line, ":", 2)
		if len(raw) < 2 {
			continue
		}
		// textIdx is the last meaningful column; split into that many parts.
		fields := strings.SplitN(raw[1], ",", textIdx+2)
		if len(fields) < textIdx+1 {
			continue
		}

		start := strings.TrimSpace(fields[startIdx])
		end := strings.TrimSpace(fields[endIdx])
		text := strings.TrimSpace(fields[textIdx])

		// Strip ASS override tags: {\an8}, {\i1}, etc.
		text = stripASSTags(text)
		// Normalise soft line-breaks (\N and \n) to a space
		text = strings.ReplaceAll(text, `\N`, " ")
		text = strings.ReplaceAll(text, `\n`, " ")
		text = strings.TrimSpace(text)

		// ASS doesn't have a sequential index; use len+1 to match SRT behaviour.
		subs = append(subs, NewSubtitle(len(subs)+1, start, end, text))
	}

	if err := scanner.Err(); err != nil {
		return subs, err
	}

	return subs, nil
}

// stripASSTags removes ASS override blocks like {\an8} or {\i1\b1}.
var assTagRE = regexp.MustCompile(`\{[^}]*\}`)

func stripASSTags(s string) string {
	return assTagRE.ReplaceAllString(s, "")
}
