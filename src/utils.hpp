#pragma once

#include <string>

namespace s2s
{

std::string Quote(const std::string &s);
std::string FormatThreeDigits(size_t n);
std::string BuildOutputPath(const std::string &media, const std::string &deck, int seq, const std::string &ext);

} // namespace s2s
