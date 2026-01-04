package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"unicode"
)

func LoadConfig() ([]AnkiField, error) {
	homeDir, err := os.UserHomeDir()
	if err != nil {
		panic(err)
	}

	configPath := filepath.Join(homeDir, Config, S2S, ConfigToml)

	if _, err := os.Stat(configPath); err != nil {
		if os.IsNotExist(err) {
			fmt.Println("Loading default config...")
			return LoadDefaultConfig()
		} else {
			return nil, err
		}
	}

	fmt.Println("Loading user config...")
	return LoadUserConfig()
}

func LoadUserConfig() ([]AnkiField, error) {
	homeDir, err := os.UserHomeDir()
	if err != nil {
		panic(err)
	}

	configPath := filepath.Join(homeDir, Config, S2S, ConfigToml)

	file, err := os.Open(configPath)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	foundLine := false
	var line string

	for scanner.Scan() {
		line = scanner.Text()
		if strings.HasPrefix(line, Order) {
			foundLine = true
			break
		}
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	if !foundLine {
		return nil, errors.New("error")
	}

	i := len(Order)

	for i < len(line) && (unicode.IsSpace(rune(line[i])) || line[i] == '[' || line[i] == '=') {
		i++
	}

	if len(line) == 0 || line[len(line)-1] != ']' {
		return nil, errors.New("error")
	}

	line = line[:len(line)-1] // remove trailing ']'

	line = line[i:] // trim prefix

	i = 0
	fields := []AnkiField{}

	for i < len(line) {
		if line[i] != '"' {
			i++
			continue
		}

		start := i + 1 // past opening quote
		end := strings.IndexByte(line[start:], '"')
		if end == -1 {
			return nil, fmt.Errorf("ERROR")
		}
		end += start

		field := line[start:end]

		theField := DetermineField(field)
		fields = append(fields, theField)

		i = end + 1 // past closing quote
	}

	return fields, nil
}

func LoadDefaultConfig() ([]AnkiField, error) {
	return nil, nil
}

func DetermineField(s string) AnkiField {
	switch s {
	case "expression":
		return Expression
	case "sequence":
		return SequenceNumber
	case "audio":
		return Audio
	case "snapshot":
		return Snapshot
	case "tags":
		return Tags
	default:
		return IDK
	}
}
