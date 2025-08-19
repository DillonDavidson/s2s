# s2s

*A tool for generating Anki decks from subtitle and video files*

`s2s` is a lightweight command-line tool that slices up video + subtitle pairs into audio clips and subtitle lines, then generates a ready-to-import Anki deck (TSV). I have only tested this on Linux for now.

---

##  Features
- C++20 implementation (with parallelization)
- Extracts audio clips aligned with subtitle timings
- Generates TSV files ready for Anki import
- Simple CLI with minimal dependencies (just )
- Currently supports: SRT subtitles, MKV video, OGG audio

---

## Requirements

- [FFmpeg](https://ffmpeg.org/) must be installed and available in your PATH
- A C++20-capable compiler
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
./build/s2s -s ~/Videos/MyVideo.srt -v ~/Videos/MyVideo.mkv -n My_Video_Deck -o MyVideoDir -t 12
```

---

## Acknowledgements

- [subs2srs](https://sourceforge.net/projects/subs2srs/) - inspiration for this project
- [btop](https://github.com/aristocratos/btop) - clean example for handling command-line arguments

---

## Contributing
Contributions are welcome! Please open an issue or pull request.

---

## License

s2s is licensed under the GNU GPL Version 3 license. [See LICENSE for more information.](https://github.com/DillonDavidson/s2s/blob/main/LICENSE)
