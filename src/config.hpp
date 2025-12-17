#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

constexpr std::string_view ORDER = "order";
constexpr std::string_view CONFIG_FILE_PATH = ".config/s2s/config.toml";

enum Fields
{
	EXPRESSION = 0,
	SEQUENCE_NUMBER = 1,
	AUDIO = 2,
	SNAPSHOT = 3,
	TAGS = 4,
	IDK = 5,
};

std::vector<Fields> LoadConfig();
std::vector<Fields> LoadUserConfig();
std::vector<Fields> LoadDefaultConfig();
Fields DetermineField(const std::string& field);
std::filesystem::path GetConfigFilePath();
