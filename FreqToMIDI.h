///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _FREQTOMIDI_H_
#define _FREQTOMIDI_H_
#include <string>
#include <vector>

// how-to for calculating midi
// https://newt.phys.unsw.edu.au/jw/notes.html
class FreqToMIDI {
public:
  std::string GetMIDI(const double& freq);

private:
  // converts from MIDI note number to string
  // example: NoteToName(12)='C 1'
  std::string NoteToName(const std::uint32_t n);
  // converts from frequency to closest MIDI note
  // example: FrequencyToNote(443)=57 (A 4)
  std::uint32_t FrequencyToNote(const double& freq);

  const double cBaseA4 = 440; // set A4=440Hz
  const std::vector<std::string> cNotes = {"C",  "C#", "D",  "D#", "E",  "F",
                                           "F#", "G",  "G#", "A",  "A#", "B"};
};

#endif //_FREQTOMIDI_H_
