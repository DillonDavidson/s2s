#include "cli.hpp"
#include "ffmpeg.hpp"
#include "srt.hpp"

int main(int argc, char *argv[])
{
	auto cli_or_ret = cli::parse(argc, argv);

	cli::Cli cli;

	if (std::holds_alternative<cli::Cli>(cli_or_ret))
	{
		cli = std::get<cli::Cli>(cli_or_ret);
	}
	else
	{
		auto ret = std::get<std::int32_t>(cli_or_ret);
		if (ret != 0)
		{
			cli::usage();
		}
		return ret;
	}

	std::vector<subtitle::Subtitle> subtitles = srt::ParseSRTFile(cli.subtitle_file);

	ffmpeg::CreateAnkiDeck(cli, subtitles);

	return 0;
}
