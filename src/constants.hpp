#pragma once

#include <filesystem>
#include <string_view>

namespace constants
{

inline constexpr int MY_BEST_GUESS = 100;
inline constexpr std::string_view ARROW = "-->";

// FFmpeg commands
inline constexpr std::string_view FFMPEG = "ffmpeg";
inline constexpr std::string_view INPUT = " -i ";
inline constexpr std::string_view DISABLE_VIDEO = " -vn";
inline constexpr std::string_view VORBIS_ENCODE = " -acodec libvorbis ";
inline constexpr std::string_view START = " -ss ";
inline constexpr std::string_view END = " -to ";
inline constexpr std::string_view COPY = " -c copy ";
inline constexpr std::string_view IMAGE = " -vframes 1 ";
inline constexpr std::string_view SCALE = " -vf \"scale=352:202\" ";

// Quiet output
inline constexpr std::string_view QUIET = " -hide_banner -loglevel error ";
inline constexpr std::string_view SHELL_BE_QUIET = " > /dev/null 2>&1";

// File stuff
inline const std::filesystem::path DEFAULT_OUTPUT_FILE = "audio.ogg";
inline constexpr std::string_view MEDIA = ".media";
inline constexpr std::string_view OGG = ".ogg";
inline constexpr std::string_view WEBP = ".webp";
inline constexpr std::string_view TSV = ".tsv";
inline constexpr std::string_view RM = "rm ";

} // namespace constants
