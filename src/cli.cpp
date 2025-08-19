#include "cli.hpp"

#include <charconv>
#include <iostream>
#include <string_view>
#include <vector>

namespace cli
{

ParseResult parse(int argc, char *argv[])
{
	const std::vector<std::string_view> args{std::next(argv, std::ptrdiff_t{1}),
	                                         std::next(argv, static_cast<std::ptrdiff_t>(argc))};

	Cli cli;

	for (auto it = args.begin(); it != args.end(); ++it)
	{
		std::string_view arg = *it;

		if (arg == "-h" || arg == "--help")
		{
			usage();
			help();
			return ParseResult{0};
		}

		if (arg == "-s" || arg == "--subtitle")
		{
			if (++it == args.end())
			{
				// Print an error message
				return ParseResult{1};
			}

			std::string_view arg = *it;
			std::filesystem::path subtitle_file = std::filesystem::path{arg};

			if (std::filesystem::is_directory(subtitle_file))
			{
				// Print error (File can't be directory)
				return ParseResult{1};
			}

			cli.subtitle_file = subtitle_file;
			continue;
		}

		if (arg == "-v" || arg == "--video")
		{
			if (++it == args.end())
			{
				// Print error (Requires an argument)
				return ParseResult{1};
			}

			std::string_view arg = *it;
			std::filesystem::path subtitle_file = std::filesystem::path{arg};

			if (std::filesystem::is_directory(subtitle_file))
			{
				// Print error (File can't be a directory)
				return ParseResult{1};
			}

			cli.video_file = arg;
			continue;
		}

		if (arg == "-o" || arg == "--output")
		{
			if (++it == args.end())
			{
				// Requires an argument, print error message
				return ParseResult{1};
			}

			std::string_view arg = *it;
			std::filesystem::path output_directory = std::filesystem::path{arg};
			cli.output_directory = output_directory;
			continue;
		}

		if (arg == "-n" || arg == "--name")
		{
			if (++it == args.end())
			{
				// Requires an argument, print error message
				return ParseResult{1};
			}

			cli.deck_name = *it;
			continue;
		}

		if (arg == "-t" || arg == "--threads")
		{
			if (++it == args.end())
			{
				// Requires an argument, print error message
				return ParseResult{1};
			}

			auto result = std::from_chars((*it).data(), (*it).data() + (*it).size(), cli.threads);
			if (result.ec == std::errc::invalid_argument)
			{
				// Could not convert, throw error
				return ParseResult{1};
			}

			continue;
		}

		// Error: Unknown argument <arg>
		return ParseResult{1};
	}

	if (cli.subtitle_file.empty())
	{
		// Error: Subtitle file is required
		return ParseResult{1};
	}

	if (cli.video_file.empty())
	{
		// Error: Video file is required
		return ParseResult{1};
	}

	if (cli.output_directory.empty())
	{
		// Error: Output directory is required
		return ParseResult{1};
	}

	if (cli.deck_name.empty())
	{
		// Error: Deck name is required
		return ParseResult{1};
	}

	return ParseResult{cli};
}

void usage()
{
	std::cout << "Usage: s2s [OPTIONS]\n\n";
}

void help()
{
	std::cout << "Options:\n"
	             "    -s, --subtitle <file>     Path to subtitle file in target "
	             "language\n"
	             "    -v, --video <file>        Path to video file used for "
	             "audio "
	             "clips\n"
	             "    -o, --output <directory>  Directory to place generated "
	             "files\n"
	             "    -n, --name                Name of generated Anki deck\n"
	             "    -t, --threads <num>       Number of threads to use (default: 1)\n"
	             "    -h, --help                Show this help message and "
	             "exit\n";
}

} // namespace cli
