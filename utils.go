package main

import (
	"errors"
	"fmt"
	"os"
)

func buildOutputPath(media, deck string, seq int, ext string) string {
	path := fmt.Sprintf("%s/%s_%03d%s", media, deck, seq, ext)
	return path
}

func fileExists(path string) bool {
	_, err := os.Stat(path)
	if err == nil {
		return true
	}

	if errors.Is(err, os.ErrNotExist) {
		return false
	}

	return false
}

func directoryExists(path string) bool {
	info, err := os.Stat(path)
	if err == nil {
		return info.IsDir()
	}

	if errors.Is(err, os.ErrNotExist) {
		return false
	}

	return false
}
