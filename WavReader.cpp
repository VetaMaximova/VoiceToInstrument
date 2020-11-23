///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "WavReader.h"

#include <cstring>
#include <iostream>

void WavReader::SetPath(const std::string &path) {
  if (sf_in) {
    sf_close(sf_in);
  }

  filename = path;
  memset(&sfinfo, 0, sizeof(sfinfo));

  sf_in = sf_open(filename.c_str(), SFM_READ, &sfinfo);
}

WavReader::~WavReader() {
  if (sf_in) {
    sf_close(sf_in);
  }
}

std::vector<double> WavReader::ReadToMono(const double milliseconds) {
  if (!sf_in) {
    std::cout << "cant open file " << filename << std::endl;
    return std::vector<double>{};
  }

  double fs = sfinfo.samplerate;
  sf_count_t n_frames = sfinfo.frames;
  sf_count_t n_samples = sfinfo.frames * sfinfo.channels;
  std::vector<double> audio(n_samples);
  sf_count_t n_read = sf_readf_double(sf_in, audio.data(), n_frames);

  if (n_read != n_frames) {
    std::cout << "cant read from file " << filename << std::endl;
    return std::vector<double>{};
  }

  if (cDebugMode) {
    std::cout << filename << " : channels " << sfinfo.channels << " frames "
              << sfinfo.frames << " fs " << fs << std::endl;
  }

  std::uint32_t number_of_frames = milliseconds > 0 ? (fs * milliseconds / 1000.0) :  sfinfo.frames;;

  std::vector<double> mono(number_of_frames);
  for (std::uint32_t i = 0; i < number_of_frames; i++) {
    float v = 0;
    for (size_t c = 0; c < (size_t)sfinfo.channels; c++) {
      v += audio[i * sfinfo.channels + c];
    }
    mono[i] = v;
  }

  return mono;
}

std::uint32_t WavReader::GetFs() { return sfinfo.samplerate; }
