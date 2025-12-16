package main

import (
	"flag"
	"fmt"
	"os"
)

type Cli struct {
	subtitleFile    string // Path to subtitle file in target language
	videoFile       string // Path to video file
	outputDirectory string // Path to subtitle file in target language
	deckName        string // Name of generated Anki deck
	verbose         bool   // Silence output of FFmpeg
	dryRun          bool   // Print commands that would be executed
	threads         int    // Number of threads to use
}

func Parse() (Cli, error) {
	flag.Usage = func() {
		f := flag.CommandLine.Output()
		fmt.Fprintln(f, "s2s - Subtitles 2 SRS")
		fmt.Fprintln(f, "")
		fmt.Fprintf(f, "Usage:  %s [OPTIONS]\n\n", os.Args[0])
		fmt.Fprintln(f, "")
		fmt.Fprintln(f, "Options:")
		flag.PrintDefaults()
	}

	var cli Cli
	var err error = nil

	flag.BoolVar(&cli.verbose, "verbose", false, "show detailed FFmpeg output and other debug info")
	flag.BoolVar(&cli.dryRun, "dry-run", false, "show what commands will run without running them")
	flag.IntVar(&cli.threads, "threads", 1, "number of threads to use")
	flag.StringVar(&cli.subtitleFile, "subtitle", "", "path to subtitle file in target language")
	flag.StringVar(&cli.videoFile, "video", "", "path to video file used for audio/video clips and images")
	flag.StringVar(&cli.outputDirectory, "output", "", "directory to place generated files")
	flag.StringVar(&cli.deckName, "name", "", "name of generated Anki deck")

	showHelp := flag.Bool("help", false, "show help")

	flag.Parse()

	if *showHelp || len(os.Args) == 1 {
		flag.Usage()
		os.Exit(0)
	}

	return cli, err
}
