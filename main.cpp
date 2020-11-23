///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include <memory.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sndfile.h>
#include <math.h>

#include "FreqCalculator.h"
#include "GetNoteData.h"
#include "FreqToMIDI.h"
#include "WavReader.h"
#include "kiss_fft.h"

int main(int argc, char **argv)
{
    std::cout << "in voice to instrument" << std::endl;
    if (argc < 4) {
      std::cerr << "you should specify input wav file\n";
      exit(1);
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];
    const char* instrument_files_path = argv[3];

    WavReader reader;
    reader.SetPath(input_path);
    auto input_data_mono = reader.ReadToMono();

    double min_freq = 20;
    double max_freq = 4200;

    FreqCalculator freq_calculator(min_freq, max_freq, reader.GetFs());
    auto freqs_with_times = freq_calculator.GetFreqsWithIntervals(input_data_mono);

    FreqToMIDI midi_generator;
    std::vector<std::pair<std::string, double>> notes_with_time;
    for(auto freq_with_time : freqs_with_times)
    {
      std::cout << "freq " << freq_with_time.first << ", time " << freq_with_time.second << std::endl;
      std::string note = midi_generator.GetMIDI(freq_with_time.first);
      std::cout << "note " << note << std::endl;
      notes_with_time.push_back(std::make_pair(note, freq_with_time.second));
    }

    std::vector<float> mono_output;
    GetNoteData instrument_notes(instrument_files_path, Instrument::guitar);

    for(auto& note_with_time: notes_with_time)
    {
      auto curr_note = instrument_notes.GetData(note_with_time.first, note_with_time.second);
      mono_output.insert( mono_output.end(), curr_note.begin(), curr_note.end() );
    }

    SF_INFO sfinfo_out;
    sfinfo_out.channels = 1;
    sfinfo_out.samplerate = reader.GetFs();
    sfinfo_out.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(output_path, SFM_WRITE, &sfinfo_out);
    if (!outfile)
    {
      std::cerr << "failed to write file" << std::endl;
    }

    sf_count_t count = sf_write_float(outfile, &mono_output[0], mono_output.size()) ;
    sf_write_sync(outfile);
    sf_close(outfile);

    std::cout << "file " << output_path << " was written" << std::endl;
    return 0;
}
