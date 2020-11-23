///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _GETFREQ_H_
#define _GETFREQ_H_
#include "kiss_fftr.h"

#include <string>
#include <vector>

enum class Algorithm { FFT, Autocorrelation };

class FreqCalculator {
public:
  FreqCalculator(const double &min_freq, const double &max_freq,
                 const double &fs);
  // returns freqs->milliseconds for all file
  std::vector<std::pair<double, double>>
  GetFreqsWithIntervals(const std::vector<kiss_fft_scalar> &signal);

private:
  std::vector<std::pair<double, double>>
  GetFFTFreqWithIntervals(const std::vector<kiss_fft_scalar> &signal);
  std::vector<kiss_fft_scalar> GetNextInterval(const std::vector<kiss_fft_scalar> &signal);
  std::uint32_t GetNextPowerOfTwo(std::uint32_t points_count);

  double min_freq, max_freq;
  std::uint32_t fs;
  std::uint32_t min_point_count;
  std::uint32_t max_point_count;
  std::uint32_t intervals_count = 0;
  std::uint32_t points_in_interval = 0;

  const Algorithm cAlgorithm = Algorithm::FFT;
  const int cMinimumIntervalRequiredMs = 300;
  const bool cDebugMode = true;
  const double cFFTFreqStep = 10;   // 10Hz
  const double cFreqHysteresis = 1; // 1Hz
};

#endif //_GETFREQ_H_
