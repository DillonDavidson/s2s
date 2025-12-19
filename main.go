package main

import (
	"log"
)

func main() {
	cli, err := Parse()
	if err != nil {
		log.Fatal(err)
	}

	subtitles, err := ParseSRTFile(cli.subtitleFile)
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
