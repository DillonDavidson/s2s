#include "ffmpeg.hpp"

#include <atomic>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "constants.hpp"
#include "utils.hpp"

static std::vector<std::string> commands;

void CreateAnkiDeck(const Cli& cli, const std::vector<Subtitle>& subtitles)
{
	if (std::filesystem::exists(DEFAULT_OUTPUT_FILE))
	{
		std::cout << "Error: " << DEFAULT_OUTPUT_FILE << " already exists. Delete it and try again.\n";
		exit(1);
	}

	if (!std::filesystem::exists(cli.output_directory))
	{
		std::cout << "Warning: Output directory does not exist. Creating '" << cli.output_directory.c_str()
		          << "'.\n";
		if (cli.dry_run)
		{
			std::cout << "mkdir " << cli.output_directory << "\n";
		}
		else
		{
			std::filesystem::create_directories(cli.output_directory);
		}
	}

	assert(std::filesystem::exists(cli.output_directory) || cli.dry_run);

	std::filesystem::path media_path = cli.output_directory / (cli.deck_name + MEDIA.data());
	if (std::filesystem::exists(media_path))
	{
		std::cout << "Error: Media path: '" << media_path << "' already exists. Delete it and try again.\n";
		exit(1);
	}

	std::ofstream tsv_file;
	if (!cli.dry_run)
	{
		std::filesystem::create_directory(media_path);
		assert(std::filesystem::exists(media_path));
		tsv_file.open(cli.output_directory / (cli.deck_name + TSV.data()));
		assert(tsv_file.is_open());
	}
	else
	{
		std::cout << "mkdir " << media_path << "\n";
		std::cout << "touch " << cli.output_directory / (cli.deck_name + TSV.data()) << "\n";
	}

	MKVtoOGG(cli, DEFAULT_OUTPUT_FILE);
	assert(std::filesystem::exists(DEFAULT_OUTPUT_FILE) || cli.dry_run);

	size_t sequence_number = 1;
	for (const auto& s : subtitles)
	{
		// ffmpeg -i audio.ogg -ss START -to END -c copy
		// <deck_name>_<sequence_number>.ogg
		GenerateAudioClip(cli, s, media_path, sequence_number);

		// make image
		GenerateImage(cli, s, media_path, sequence_number);

		// write to tsv file
		WriteToOutputTSV(cli, s, tsv_file, sequence_number);

		sequence_number++;
	}

	tsv_file.close();

	if (cli.dry_run)
	{
		for (const auto& cmd : commands)
		{
			std::cout << cmd << "\n";
		}

		std::cout << (RM.data() + DEFAULT_OUTPUT_FILE.string()) << "\n";
	}
	else
	{
		RunInParallel(cli.threads);
		std::system((RM.data() + DEFAULT_OUTPUT_FILE.string()).c_str());
	}
}

void MKVtoOGG(const Cli& cli, const std::filesystem::path& output_file)
{
	std::ostringstream command;

	command << FFMPEG;

	if (cli.be_quiet)
	{
		command << QUIET;
	}

	command << INPUT << Quote(cli.video_file.c_str()) << DISABLE_VIDEO;
	command << VORBIS_ENCODE << Quote(output_file.c_str());

	if (cli.be_quiet)
	{
		command << SHELL_BE_QUIET;
	}

	if (cli.dry_run)
	{
		std::cout << command.str() << "\n";
	}
	else
	{
		std::system(command.str().c_str());
	}
}

void GenerateAudioClip(const Cli& cli, const Subtitle& s, const std::filesystem::path& media_path, size_t seq)
{
	// ffmpeg -i audio.ogg -ss START -to END -c copy
	// <deck_name>_<sequence_number>.ogg
	std::ostringstream command;

	command << FFMPEG;
	if (cli.be_quiet)
	{
		command << QUIET;
	}

	command << INPUT << DEFAULT_OUTPUT_FILE.c_str() << START << s.start << END << s.end << COPY;
	command << BuildOutputPath(media_path.c_str(), cli.deck_name.data(), seq, OGG.data());

	if (cli.be_quiet)
	{
		command << SHELL_BE_QUIET;
	}

	commands.push_back(command.str());
}

void GenerateImage(const Cli& cli, const Subtitle& s, const std::filesystem::path& media_path, size_t seq)
{
	// ffmpeg -ss 0:0:3.227 -i 01.mkv -vframes 1 01.webp
	std::ostringstream command;

	command << FFMPEG;

	if (cli.be_quiet)
	{
		command << QUIET;
	}

	command << START << s.start << INPUT << Quote(cli.video_file.c_str()) << IMAGE << SCALE;
	command << BuildOutputPath(media_path, cli.deck_name, seq, WEBP.data());

	if (cli.be_quiet)
	{
		command << SHELL_BE_QUIET;
	}

	commands.push_back(command.str());
}

void WriteToOutputTSV(const Cli& cli, const Subtitle& s, std::ofstream& tsv_file, size_t seq)
{
	if (cli.dry_run)
	{
		return;
	}

	std::string format_seq = FormatThreeDigits(seq);
	tsv_file << cli.deck_name << "\t";
	tsv_file << format_seq << "\t";
	tsv_file << "[sound:" << cli.deck_name << "_" << format_seq;
	tsv_file << OGG << "]\t";
	tsv_file << "<img src=\"" << cli.deck_name << "_" << format_seq;
	tsv_file << WEBP << "\">\t";
	tsv_file << s.text << "\n";
}

void RunInParallel(size_t num_threads)
{
	std::atomic_size_t index{0};

	auto worker = [&]
	{
		while (true)
		{
			size_t i = index.fetch_add(1);
			if (i >= commands.size())
			{
				break;
			}

			int ret = std::system(commands[i].c_str());
			if (ret != 0)
			{
				std::cerr << "FFmpeg command failed: " << commands[i] << "\n";
			}
		}
	};

	std::vector<std::thread> threads;
	for (size_t t = 0; t < num_threads; ++t)
	{
		threads.emplace_back(worker);
	}

	for (auto& th : threads)
	{
		th.join();
	}
}
