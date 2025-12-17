#include "config.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

std::vector<Fields> LoadConfig()
{
	if (std::filesystem::exists(GetConfigFilePath()))
	{
		return LoadUserConfig();
	}

	return LoadDefaultConfig();
}

std::vector<Fields> LoadUserConfig()
{
	std::ifstream config_file(GetConfigFilePath());
	if (!config_file.is_open())
	{
		std::cerr << "Warning: Failed to read config file at '" << GetConfigFilePath()
		          << "' so loading defaults.\n";
		return LoadDefaultConfig();
	}

	std::string line;
	bool found_line = false;
	while (std::getline(config_file, line))
	{
		if (line.starts_with(ORDER))
		{
			found_line = true;
			break;
		}
	}

	if (!found_line)
	{
		throw std::invalid_argument("ERROR");
	}

	std::size_t i = ORDER.size();

	while (std::isspace(line[i]) || line[i] == '[' || line[i] == '=')
	{
		++i;
	}

	if (line.back() == ']')
	{
		line.pop_back();
	}
	else
	{
		throw std::invalid_argument("ERROR");
	}

	line = line.substr(i);

	i = 0;
	std::vector<Fields> fields;
	std::string field;

	while (i < line.size())
	{
		if (line[i] != '\"')
		{
			++i;
			continue;
		}

		size_t start = ++i; // move past the opening quotation mark
		size_t end = line.find('\"', start);
		if (end == std::string::npos)
		{
			throw std::invalid_argument("ERROR");
		}

		field = line.substr(start, end - start);

		Fields the_field = DetermineField(field);
		fields.push_back(the_field);

		i = end + 1; // move past the closing quotation mark
	}

	return fields;
}

std::vector<Fields> LoadDefaultConfig()
{
	return {};
}

Fields DetermineField(const std::string& field)
{
	if (field == "expression")
	{
		return EXPRESSION;
	}

	if (field == "sequence")
	{
		return SEQUENCE_NUMBER;
	}

	if (field == "audio")
	{
		return AUDIO;
	}

	if (field == "snapshot")
	{
		return SNAPSHOT;
	}

	if (field == "tags")
	{
		return TAGS;
	}

	return IDK;
}

std::filesystem::path GetConfigFilePath()
{
	const char* home = std::getenv("HOME");
	return std::filesystem::path(home) / CONFIG_FILE_PATH;
}
