#pragma once

#include <filesystem>
#include <vector>

#include "subtitle.hpp"

std::vector<Subtitle> ParseSRTFile(const std::filesystem::path& path);
