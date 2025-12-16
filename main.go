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

	subtitles := ParseSRTFile(cli.subtitleFile)

	fmt.Println("Subtitle File:    " + cli.subtitleFile)
	fmt.Println("Video File:       " + cli.videoFile)
	fmt.Println("Deck Name:        " + cli.deckName)
	fmt.Println("Output Directory: " + cli.outputDirectory)
}
