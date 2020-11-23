///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "GetNoteData.h"

#include <iostream>
#include <memory.h>

GetNoteData::GetNoteData(const std::string &path_to_instrument_files,
                         const Instrument &instrument)
    : path_to_instrument_files(path_to_instrument_files),
      selected_instrument(instrument) {
  selected_path = path_to_instrument_files + "/" +
                  InstrumentToString(selected_instrument) + "/";

  if (cDebugMode) {
    std::cout << "I would read data from " << selected_path << std::endl;
  }
}

inline const char *GetNoteData::InstrumentToString(Instrument instrument) {
  switch (instrument) {
  case Instrument::guitar:
    return "guitar";
  case Instrument::piano:
  default:
    return "piano";
  }
}

std::vector<double> GetNoteData::GetData(const std::string &note,
                                        const double &milliseconds) {
  std::string filename = selected_path + note + ".wav";
  reader.SetPath(filename);
  return reader.ReadToMono(milliseconds);
}
