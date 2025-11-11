#pragma once

#include "cli.hpp"
#include "subtitle.hpp"

#include <filesystem>
#include <vector>

// I think FFmpeg will use Vorbis-encoded OGG by default, but specify it anyway

namespace s2s
{

void CreateAnkiDeck(const Cli &cli, const std::vector<Subtitle> &subtitles);
void MKVtoOGG(const Cli &cli, const std::filesystem::path &output_file);
void GenerateAudioClip(const Cli &cli, const Subtitle &s, const std::filesystem::path &media_path, size_t seq);
void GenerateImage(const Cli &cli, const Subtitle &s, const std::filesystem::path &media_path, size_t seq);
void WriteToOutputTSV(const Cli &cli, const Subtitle &s, std::ofstream &tsv_file, size_t seq);
void RunInParallel(size_t num_threads);

} // namespace s2s
