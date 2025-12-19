# s2s

*A tool for generating Anki decks from subtitle and video files*

`s2s` is a lightweight command-line tool that slices up video + subtitle pairs into audio clips and subtitle lines, then generates a TSV file for importing as an Anki deck.

---

##  Features
- Extract audio clips aligned with subtitle timings
- Generates TSV files ready for Anki import
- Simple CLI with minimal dependencies
- Currently supports: SRT subtitles, MKV video, OGG audio, with plans to expand

---

## Requirements

- [FFmpeg](https://ffmpeg.org/) must be installed and available in your PATH

---

## Building

```bash
git clone https://github.com/DillonDavidson/s2s
cd s2s
go build
./s2s # commands
```

---

## Usage

```text
Options:
  -dry-run
        show what commands will run without running them
  -help
        show help
  -name string
        name of generated Anki deck
  -output string
        directory to place generated files
  -subtitle string
        path to subtitle file in target language
  -threads int
        number of threads to use (default 1)
  -verbose
        show detailed FFmpeg output and other debug info
  -video string
        path to video file used for audio/video clips and images
```

---

## Example

```bash
./s2s -subtitle sub.srt -video vid.mkv -output ./videos -name my_deck
```

---

## To-Do List

- [ ] Support for a configuration file
- [ ] Customizable gap before and after subtitle times
- [ ] Customizable file types
- [ ] Generating video clips
- [ ] Customizable snapshot dimensions 

---

## License

s2s is licensed under the GNU GPL Version 3 license. [See LICENSE for more information.](https://github.com/DillonDavidson/s2s/blob/main/LICENSE)
