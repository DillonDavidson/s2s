package main

import (
	"errors"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
)

var commands [][]string

func MKVtoOGG(cli Cli, outputFile string) error {
	args := []string{}

	if !cli.verbose {
		args = append(args, QuietOne, QuietTwo, QuietThree)
	}

	args = append(args, Input, cli.videoFile, DisableVideo, outputFile)

	cmd := exec.Command("ffmpeg", args...)

	if cli.dryRun {
		fmt.Println(cmd)
		return nil
	}

	if cli.verbose {
		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr
	} else {
		cmd.Stdout = io.Discard
		cmd.Stderr = io.Discard
	}

	cmd.Stdin = os.Stdin

	return cmd.Run()
}

func GenerateAudioClip(cli Cli, s Subtitle, mediaPath string, seq int) {
	// ffmpeg -i audio.ogg -ss START -to END -c copy
	// <deck_name>_<sequence_number>.ogg
	command := []string{}

	command = append(command, FFmpeg)

	if !cli.verbose {
		command = append(command, QuietOne, QuietTwo, QuietThree)
	}

	command = append(command, Input, DefaultOutputFile, Start, s.start, End, s.end, CopyOne, CopyTwo)
	command = append(command, buildOutputPath(mediaPath, cli.deckName, seq, Ogg))

	commands = append(commands, command)
}

func GenerateImage(cli Cli, s Subtitle, mediaPath string, seq int) {
	// ffmpeg -ss 0:0:3.227 -i 01.mkv -vframes 1 01.webp
	command := []string{}

	command = append(command, FFmpeg)

	if !cli.verbose {
		command = append(command, QuietOne, QuietTwo, QuietThree)
	}

	command = append(command, Start, s.start, Input, cli.videoFile, ImageOne, ImageTwo, ScaleOne, ScaleTwo)
	command = append(command, buildOutputPath(mediaPath, cli.deckName, seq, Webp))

	commands = append(commands, command)
}

func WriteToOutputTSV(cli Cli, s Subtitle, tsvFile *os.File, seq int) {
	if cli.dryRun {
		return
	}

	formatSeq := fmt.Sprintf("%03d", seq)
	fmt.Fprintf(tsvFile, "%s\t%s\t[sound:%s_%s%s]\t<img src=\"%s_%s%s\">\t%s\n",
		cli.deckName,
		formatSeq,
		cli.deckName,
		formatSeq,
		Ogg,
		cli.deckName,
		formatSeq,
		Webp,
		s.text,
	)
}

func CreateAnkiDeck(cli Cli, subtitles []Subtitle) error {
	if fileExists(DefaultOutputFile) {
		return errors.New("error: " + DefaultOutputFile + " already exists, delete it and try again")
	}

	if !directoryExists(cli.outputDirectory) {
		fmt.Println("warning: output directory does not exist, so creating " + cli.outputDirectory)

		if cli.dryRun {
			fmt.Println("mkdir " + cli.outputDirectory)
		} else {
			err := os.MkdirAll(cli.outputDirectory, 0o755)
			if err != nil {
				return err
			}
		}
	}

	if !directoryExists(cli.outputDirectory) && !cli.dryRun {
		return errors.New("error: output directory doesn't exist and not a dry run")
	}

	mediaPath := filepath.Join(cli.outputDirectory, (cli.deckName + Media))
	if fileExists(mediaPath) {
		return errors.New("error: media path '" + mediaPath + "' already exists. delete it and try again")
	}

	var tsvFile *os.File
	if !cli.dryRun {
		if err := os.MkdirAll(mediaPath, 0o755); err != nil {
			return err
		}

		tsvPath := filepath.Join(
			cli.outputDirectory,
			cli.deckName+Tsv,
		)

		tsvFile, err := os.Create(tsvPath)
		if err != nil {
			return err
		}
		defer tsvFile.Close()

	} else {
		fmt.Println("mkdir", mediaPath)
		fmt.Println("touch", filepath.Join(cli.outputDirectory, cli.deckName+Tsv))
	}

	err := MKVtoOGG(cli, DefaultOutputFile)
	if err != nil {
		return err
	}

	if !fileExists(DefaultOutputFile) && !cli.dryRun {
		return errors.New("error: '" + DefaultOutputFile + "' doesn't exist and not a dry run")
	}

	for i := range subtitles {
		// ffmpeg -i audio.ogg -ss START -to END -c copy
		// <deck_name>_<sequence_number>.ogg
		GenerateAudioClip(cli, subtitles[i], mediaPath, i+1)

		// make image
		GenerateImage(cli, subtitles[i], mediaPath, i+1)

		// write to tsv file
		WriteToOutputTSV(cli, subtitles[i], tsvFile, i+1)
	}

	for _, args := range commands {
		cmd := exec.Command(args[0], args[1:]...)
		if cli.dryRun {
			fmt.Println(cmd)
		} else {
			cmd.Stdout = os.Stdout
			cmd.Stderr = os.Stderr
			cmd.Stdin = os.Stdin

			if err := cmd.Run(); err != nil {
				fmt.Println(cmd)
				return err
			}
		}
	}

	cmd := exec.Command(Rm, DefaultOutputFile)
	if cli.dryRun {
		fmt.Println(cmd)
	} else {
		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr
		cmd.Stdin = os.Stdin

		if err := cmd.Run(); err != nil {
			return err
		}
	}

	return nil
}
