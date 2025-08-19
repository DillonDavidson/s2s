#pragma once

#include <filesystem>
#include <vector>

#include "subtitle.hpp"

namespace srt
{

std::vector<subtitle::Subtitle> ParseSRTFile(const std::filesystem::path &path);

} // namespace srt
