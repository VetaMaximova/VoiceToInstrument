///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _WAVREADER_H_
#define _WAVREADER_H_
#include <map>
#include <sndfile.h>
#include <string>
#include <vector>

class WavReader {
public:
  ~WavReader();
  void SetPath(const std::string &path);
  std::vector<double> ReadToMono(const double milliseconds = 0);
  std::uint32_t GetFs();

private:
  std::string filename;
  SNDFILE *sf_in = nullptr;
  SF_INFO sfinfo;

  const bool cDebugMode = true;
};
#endif //_WAVREADER_H_
