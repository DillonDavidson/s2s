#pragma once

#include <cstdint>
#include <string>

namespace subtitle
{

struct Subtitle
{
	Subtitle(uint32_t idx, const std::string &s, const std::string &e, const std::string &t)
	    : index(idx), start(s), end(e), text(t)
	{
	}

	uint32_t index = 0;
	std::string start;
	std::string end;
	std::string text;
};

} // namespace subtitle
