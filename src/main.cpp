#include "cli.hpp"
#include "ffmpeg.hpp"
#include "srt.hpp"

int main(int argc, char* argv[])
{
	Cli cli = Parse(argc, argv);

	if (cli.error_code != 0)
	{
		return cli.error_code;
	}

	std::vector<Subtitle> subtitles = ParseSRTFile(cli.subtitle_file);

	CreateAnkiDeck(cli, subtitles);

	return 0;
}
