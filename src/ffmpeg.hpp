#pragma once

#include <filesystem>
#include <vector>

#include "cli.hpp"
#include "subtitle.hpp"

// I think FFmpeg will use Vorbis-encoded OGG by default, but specifiy anyway

namespace ffmpeg
{

using subtitle::Subtitle;

void CreateAnkiDeck(const cli::Cli &cli, const std::vector<Subtitle> &subtitles);
void MKVtoOGG(const cli::Cli &cli, const std::filesystem::path &output_file);
void GenerateAudioClip(const cli::Cli &cli, const Subtitle &s, const std::filesystem::path &media_path, size_t seq);
void GenerateImage(const cli::Cli &cli, const Subtitle &s, const std::filesystem::path &media_path, size_t seq);
void WriteToOutputTSV(const cli::Cli &cli, const Subtitle &s, std::ofstream &tsv_file, size_t seq);
void RunInParallel(size_t num_threads);

} // namespace ffmpeg
