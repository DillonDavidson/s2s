#include "srt.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>

#include "constants.hpp"
#include "subtitle.hpp"

std::vector<Subtitle> ParseSRTFile(const std::filesystem::path& path)
{
	std::vector<Subtitle> subs;
	std::string line, text, start, end;
	int part = 0, index = 0;
	size_t arrowPosition = 0;

	std::ifstream file(path.c_str());
	assert(file.is_open());
	text.reserve(MY_BEST_GUESS);

	while (std::getline(file, line))
	{
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

		if (!line.empty())
		{
			if (part == 0)
			{
				index = std::atoi(line.c_str());
				part++;
				continue;
			}

			arrowPosition = line.find("-->");

			if (arrowPosition != std::string::npos)
			{
				start = line.substr(0, arrowPosition - 1);
				end = line.substr(arrowPosition + 4);
				std::replace(start.begin(), start.end(), ',', '.');
				std::replace(end.begin(), end.end(), ',', '.');
			}
			else
			{
				if (!text.empty())
				{
					text.append(" ");
				}

				text.append(line);
			}

			part++;
		}

		if (line.empty() || file.peek() == EOF)
		{
			Subtitle subtitle(index, start, end, text);
			subs.push_back(subtitle);
			part = 0;
			text.clear();
		}
	}

	return subs;
}
