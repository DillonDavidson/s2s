#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <variant>

namespace cli
{

struct Cli
{
	// Path to subtitle file in target language
	std::filesystem::path subtitle_file;
	// Path to video file
	std::filesystem::path video_file;
	// Path to subtitle file in target language
	std::filesystem::path output_directory;
	// Name of generated Anki deck
	std::string deck_name;
	bool be_quiet = true;
	int threads = 1; // Maybe bad to use an int?
};

using ParseResult = std::variant<Cli, std::int32_t>;

ParseResult parse(int argc, char *argv[]);
void usage();
void help();

} // namespace cli
