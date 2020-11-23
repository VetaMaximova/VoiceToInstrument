///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "FreqToMIDI.h"

#include <math.h>

std::string FreqToMIDI::NoteToName(const std::uint32_t n) {
  if (n >= 0 && n <= 119) {
    return cNotes[n % 12] + std::to_string(n / 12);
  } else
    return "---";
}

std::uint32_t FreqToMIDI::FrequencyToNote(const double& freq) {
  return (12.0 * log2(freq / cBaseA4)) + 57;
}

std::string FreqToMIDI::GetMIDI(const double& freq) {
  std::uint32_t note = FrequencyToNote(freq);
  return NoteToName(note);
}
