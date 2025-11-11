#pragma once

#include <filesystem>
#include <string>

namespace s2s
{

constexpr std::string_view USAGE = "Options:\n"
                                   "    -s, --subtitle <file>     Path to subtitle file in target language\n"
                                   "    -v, --video <file>        Path to video file used for audio clips\n"
                                   "    -o, --output <directory>  Directory to place generated files\n"
                                   "    -n, --name                Name of generated Anki deck\n"
                                   "    -t, --threads <num>       Number of threads to use (default: 1)\n"
                                   "    --dry-run                 Show what commands will run without running them"
                                   "    --verbose                 Show detailed FFmpeg output and other debug info"
                                   "    -h, --help                Show this help message and exit\n";

struct Cli
{
	std::filesystem::path subtitle_file;    // Path to subtitle file in target language
	std::filesystem::path video_file;       // Path to video file
	std::filesystem::path output_directory; // Path to subtitle file in target language
	std::string deck_name;                  // Name of generated Anki deck
	bool be_quiet = true;                   // Silence output of FFmpeg
	bool dry_run = false;                   // Print commands that would be executed
	unsigned int threads = 1;               // Number of threads to use
	int error_code = 0;                     // Error code, if any
};

Cli Parse(int argc, char *argv[]);
void Usage();

} // namespace s2s
