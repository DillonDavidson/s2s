#pragma once

#include <string>

namespace utils
{

std::string Quote(const std::string &s);
std::string FormatThreeDigits(size_t n);
std::string BuildOutputPath(const std::string &media, const std::string &deck, int seq, const std::string &ext);

} // namespace utils
