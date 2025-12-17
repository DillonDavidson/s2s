package main

import (
	"fmt"
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

	fmt.Println("Subtitle File:    " + cli.subtitleFile)
	fmt.Println("Video File:       " + cli.videoFile)
	fmt.Println("Deck Name:        " + cli.deckName)
	fmt.Println("Output Directory: " + cli.outputDirectory)
}
