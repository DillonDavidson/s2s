#pragma once

#include <filesystem>
#include <vector>

#include "subtitle.hpp"

namespace s2s
{

std::vector<Subtitle> ParseSRTFile(const std::filesystem::path &path);

} // namespace s2s
