#include "cli.hpp"
#include "ffmpeg.hpp"
#include "srt.hpp"

using namespace s2s;

int main(int argc, char *argv[])
{
	s2s::Cli cli = s2s::Parse(argc, argv);

	if (cli.error_code != 0)
	{
		return cli.error_code;
	}

	std::vector<Subtitle> subtitles = ParseSRTFile(cli.subtitle_file);

	CreateAnkiDeck(cli, subtitles);

	return 0;
}
