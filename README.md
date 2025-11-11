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
- A C++ compiler (I like Clang++)
- Meson + Ninja build system

---

## Building

```bash
git clone https://github.com/DillonDavidson/s2s
cd s2s
meson setup build
ninja -C build
./build/s2s # commands
```

---

## Usage

```text
Options:
    -s, --subtitle <file>     Path to subtitle file in target language
    -v, --video <file>        Path to video file used for audio clips
    -o, --output <directory>  Directory to place generated files
    -n, --name <string>       Name of generated Anki deck
    -t, --threads <num>       Number of threads to use (default: 1)
    -h, --help                Show this help message and exit
```

---

## Example

```bash
./build/s2s -s sub.srt -v vid.mkv -n my_deck -o ./my_vids/
```

---

## License

s2s is licensed under the GNU GPL Version 3 license. [See LICENSE for more information.](https://github.com/DillonDavidson/s2s/blob/main/LICENSE)
