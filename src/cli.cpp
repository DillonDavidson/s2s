#include "cli.hpp"

#include <charconv>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

namespace s2s
{

Cli Parse(int argc, char *argv[])
{
	const std::vector<std::string_view> args(argv + 1, argv + argc);

	Cli cli{};

	cli.be_quiet = true;
	cli.dry_run = false;
	cli.threads = 1;
	cli.error_code = 0;

	std::error_code ec;

	for (auto it = args.begin(); it != args.end(); ++it)
	{
		const std::string_view arg = *it;

		if (arg == "-h" || arg == "--help")
		{
			Usage();
			cli.error_code = 1;
			return cli;
		}

		if (arg == "-s" || arg == "--subtitle")
		{
			if (++it == args.end())
			{
				std::cerr << "Error: Missing subtitle file after '" << (it - 1)->data() << "'.\n";
				cli.error_code = 1;
				return cli;
			}

			std::string_view arg = *it;
			std::filesystem::path sub_file = std::filesystem::path{arg};

			if (std::filesystem::is_regular_file(sub_file, ec))
			{
				cli.subtitle_file = sub_file;
				continue;
			}

			std::cout << "Error: '" << sub_file << "' is not a file.\n";
			cli.error_code = 1;
			return cli;
		}

		if (arg == "-v" || arg == "--video")
		{
			if (++it == args.end())
			{
				std::cerr << "Error: Missing video file after '" << (it - 1)->data() << "'.\n";
				cli.error_code = 1;
				return cli;
			}

			std::string_view arg = *it;
			std::filesystem::path vid_file = std::filesystem::path{arg};

			if (std::filesystem::is_regular_file(vid_file, ec))
			{
				cli.video_file = vid_file;
				continue;
			}

			std::cout << "Error: '" << vid_file << "' is not a file.\n";
			cli.error_code = 1;
			return cli;
		}

		if (arg == "-o" || arg == "--output")
		{
			if (++it == args.end())
			{
				std::cerr << "Error: Missing output directory after '" << (it - 1)->data() << "'.\n";
				cli.error_code = 1;
				return cli;
			}

			std::string_view arg = *it;
			std::filesystem::path out_dir = std::filesystem::path{arg};

			if (std::filesystem::is_regular_file(out_dir, ec))
			{
				std::cout << "Error: '" << out_dir << "' is not a directory.\n";
				cli.error_code = 1;
				return cli;
			}

			cli.output_directory = out_dir;
			continue;
		}

		if (arg == "-n" || arg == "--name")
		{
			if (++it == args.end())
			{
				std::cerr << "Error: Missing name after '" << (it - 1)->data() << "'.\n";
				cli.error_code = 1;
				return cli;
			}

			cli.deck_name = *it;
			continue;
		}

		if (arg == "-t" || arg == "--threads")
		{
			if (++it == args.end())
			{
				std::cerr << "Error: Missing thread count after '" << (it - 1)->data() << "'.\n";
				cli.error_code = 1;
				return cli;
			}

			auto result = std::from_chars((*it).data(), (*it).data() + (*it).size(), cli.threads);
			if (result.ec == std::errc::invalid_argument)
			{
				std::cerr << "Error: Thread count is invalid.\n";
				cli.error_code = 1;
				return cli;
			}

			continue;
		}

		if (arg == "--dry-run")
		{
			cli.dry_run = true;
			continue;
		}

		if (arg == "--verbose")
		{
			cli.be_quiet = false;
			continue;
		}

		std::cerr << "Error: Unknown argument '" << arg << "'.\n";
		cli.error_code = 1;
		return cli;
	}

	if (cli.subtitle_file.empty())
	{
		std::cerr << "Error: Subtitle file is required.\n";
		cli.error_code = 1;
		return cli;
	}

	if (cli.video_file.empty())
	{
		std::cerr << "Error: Video file is required.\n";
		cli.error_code = 1;
		return cli;
	}

	if (cli.output_directory.empty())
	{
		std::cerr << "Error: Output directory is required.\n";
		cli.error_code = 1;
		return cli;
	}

	if (cli.deck_name.empty())
	{
		std::cerr << "Error: Deck name is required.\n";
		cli.error_code = 1;
		return cli;
	}

	return cli;
}

void Usage()
{
	std::cout << "Usage: s2s [OPTIONS]\n\n";
	std::cout << USAGE;
}

} // namespace s2s
