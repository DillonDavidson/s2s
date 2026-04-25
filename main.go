package main

import (
	"log"
	"os"
)

func main() {
	cli, err := Parse()
	if err != nil {
		log.Fatal(err)
	}

	if cli.batch {
		if err := DoBatch(cli); err != nil {
			log.Fatal(err)
		}
		os.Exit(0)
	}

	subtitles, err := ParseSubtitleFile(cli.subtitleFile)
	if err != nil {
		log.Fatal(err)
	}
	if len(subtitles) == 0 {
		log.Fatal("no subtitles read")
	}

	err = CreateAnkiDeck(cli, subtitles)
	if err != nil {
		log.Fatal(err)
	}
}
