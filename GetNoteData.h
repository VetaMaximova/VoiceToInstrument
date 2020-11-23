///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _GETNOTEDATA_H_
#define _GETNOTEDATA_H_
#include "WavReader.h"

#include <sndfile.h>
#include <string>
#include <vector>

enum class Instrument { guitar, piano };

// Get vector with specific note for selected instrument
class GetNoteData {
public:
  GetNoteData(const std::string &path_to_instrument_files,
              const Instrument &instrument);
  std::vector<double> GetData(const std::string &note,
                             const double &milliseconds);

private:
  inline const char *InstrumentToString(Instrument instrument);

  std::string path_to_instrument_files;
  std::string selected_path;
  Instrument selected_instrument;
  WavReader reader;

  const bool cDebugMode = true;
};

#endif //_GETNOTEDATA_H_
