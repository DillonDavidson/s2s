package main

import (
	"flag"
	"fmt"
	"os"
	"strings"
)

type Cli struct {
	subtitleFile    string // Path to subtitle file in target language
	videoFile       string // Path to video file
	outputDirectory string // Path to subtitle file in target language
	deckName        string // Name of generated Anki deck
	verbose         bool   // Silence output of FFmpeg
	dryRun          bool   // Print commands that would be executed
	threads         int    // Number of threads to use
	batch           bool   // Process all matching video+subtitle pairs in cwd
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

	flag.StringVar(&cli.subtitleFile, "subtitle", "", "path to subtitle file in target language")
	flag.StringVar(&cli.videoFile, "video", "", "path to video file used for audio/video clips and images")
	flag.StringVar(&cli.outputDirectory, "output", "", "directory to place generated files")
	flag.StringVar(&cli.deckName, "name", "", "name of generated Anki deck")
	flag.BoolVar(&cli.verbose, "verbose", false, "show detailed FFmpeg output and other debug info")
	flag.BoolVar(&cli.dryRun, "dry-run", false, "show what commands will run without running them")
	flag.IntVar(&cli.threads, "threads", 1, "number of threads to use")
	flag.BoolVar(&cli.batch, "batch", false, "process all matching video+subtitle pairs in cwd")

	showHelp := flag.Bool("help", false, "show help")

	flag.Parse()

	if *showHelp || len(os.Args) == 1 {
		flag.Usage()
		os.Exit(0)
	}

	if cli.batch && (cli.videoFile != "" || cli.subtitleFile != "") {
		fmt.Fprintln(os.Stderr, "Error: --batch cannot be used with -video or -subtitle")
		os.Exit(1)
	}

	if !cli.batch {
		ValidateRequired(cli)
	}

	return cli, err
}

func ValidateRequired(cli Cli) {
	checks := []struct {
		value string
		flag  string
	}{
		{cli.subtitleFile, "-subtitle"},
		{cli.videoFile, "-video"},
		{cli.outputDirectory, "-output"},
		{cli.deckName, "-name"},
	}

	var missing []string

	for _, c := range checks {
		if c.value == "" {
			missing = append(missing, c.flag)
		}
	}

	if len(missing) > 0 {
		fmt.Fprintln(os.Stderr, "Error: Missing required flags:", strings.Join(missing, ", ")+"\n")
		flag.Usage()
		os.Exit(1)
	}
}
