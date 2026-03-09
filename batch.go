package main

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
)

type Pair struct {
	Video    string
	Subtitle string
}

func FindPairs(dir string) ([]Pair, error) {
	entries, err := os.ReadDir(dir)
	if err != nil {
		return nil, err
	}

	// maps base name → filename
	subs := map[string]string{}
	videos := map[string]string{}

	for _, e := range entries {
		if e.IsDir() {
			continue
		}

		name := e.Name()
		ext := strings.ToLower(filepath.Ext(name))
		base := strings.TrimSuffix(name, ext)

		switch ext {
		case ".srt":
			subs[base] = filepath.Join(dir, name)
		case ".mkv":
			videos[base] = filepath.Join(dir, name)
		}
	}

	var pairs []Pair
	for base, video := range videos {
		if sub, ok := subs[base]; ok {
			pairs = append(pairs, Pair{
				Video:    video,
				Subtitle: sub,
			})
		}
	}

	// sorting is probably not necessary, but eh whatever
	sort.Slice(pairs, func(i, j int) bool {
		return pairs[i].Video < pairs[j].Video
	})

	return pairs, nil
}

func DeriveBatchDeckName(videoPath string) string {
	// base name of the video file without extension
	base := strings.TrimSuffix(filepath.Base(videoPath), filepath.Ext(videoPath))

	// name of the current working directory
	dir, err := os.Getwd()
	if err != nil {
		// fallback if something goes wrong
		dir = "Show"
	}
	dirName := filepath.Base(dir)

	// combine: <directory-name><file-base>
	return dirName + base
}

func DoBatch(cli Cli) error {
	pairs, err := FindPairs(".")
	if err != nil {
		return err
	}

	// derive base output dir, e.g., ~/Desktop/<current folder>
	home, _ := os.UserHomeDir()
	batchOutputDir := filepath.Join(home, "Desktop", filepath.Base("."))

	for i, pair := range pairs {
		// copy cli so original struct isn't modified
		job := cli
		job.videoFile = pair.Video
		job.subtitleFile = pair.Subtitle
		job.deckName = DeriveBatchDeckName(pair.Video)
		job.outputDirectory = batchOutputDir

		fmt.Printf("[%d/%d] Processing %s → deck: %s\n", i+1, len(pairs), pair.Video, job.deckName)

		subtitles, err := ParseSRTFile(job.subtitleFile)
		if err != nil {
			return err
		}

		if err := CreateAnkiDeck(job, subtitles); err != nil {
			return err
		}
	}

	return nil
}
