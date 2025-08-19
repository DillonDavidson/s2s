#include "utils.hpp"

namespace utils
{

std::string Quote(const std::string &s)
{
	return "\"" + s + "\"";
}

std::string FormatThreeDigits(size_t n)
{
	if (n > 99)
	{
		return std::to_string(n);
	}

	if (n > 9)
	{
		return "0" + std::to_string(n);
	}

	return "00" + std::to_string(n);
}

std::string BuildOutputPath(const std::string &media, const std::string &deck, int seq, const std::string &ext)
{
	return Quote(media + "/" + deck + "_" + FormatThreeDigits(seq) + ext);
}

} // namespace utils
