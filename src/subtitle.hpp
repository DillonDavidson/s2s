#pragma once

#include <string>

struct Subtitle
{
	Subtitle(unsigned int idx, const std::string& s, const std::string& e, const std::string& t)
	    : index(idx),
	      start(s),
	      end(e),
	      text(t)
	{
	}

	unsigned int index = 0;
	std::string start;
	std::string end;
	std::string text;
};
