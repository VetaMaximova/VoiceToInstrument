///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "FreqCalculator.h"

#include <algorithm>
#include <iostream>
#include <numeric>

FreqCalculator::FreqCalculator(const double &min_freq, const double &max_freq,
                               const double &fs)
    : min_freq(min_freq), max_freq(max_freq), fs(fs) {
  min_point_count = fs / max_freq;
  max_point_count = fs / min_freq;

  if (cDebugMode) {
    std::cout << "min T " << min_point_count << " max T " << max_point_count
              << std::endl;
  }
}

std::vector<kiss_fft_scalar>
FreqCalculator::GetNextInterval(const std::vector<kiss_fft_scalar> &signal) {
  std::uint32_t current_interval = intervals_count;
  intervals_count++;
  if ((current_interval + 1) * points_in_interval >= signal.size())
    return std::vector<kiss_fft_scalar>{};
  return std::vector<kiss_fft_scalar>{
      signal.begin() + current_interval * points_in_interval,
      signal.begin() + (current_interval + 1) * points_in_interval};
}

std::uint32_t FreqCalculator::GetNextPowerOfTwo(std::uint32_t points_count) {
  std::uint32_t power = 1;
  while (power < points_count) {
    power *= 2;
  }
  return power;
}

std::vector<std::pair<double, double>>
FreqCalculator::GetFFTFreqWithIntervals(const std::vector<kiss_fft_scalar> &signal) {
  points_in_interval = GetNextPowerOfTwo(fs / cFFTFreqStep);
  double calculated_freq_step = (double)fs / points_in_interval;

  if (cDebugMode) {
    std::cout << "pcnt " << points_in_interval << ", freq step "
              << calculated_freq_step << std::endl;
  }

  std::vector<kiss_fft_scalar> next_signal = GetNextInterval(signal);
  std::vector<double> freqs;

  kiss_fftr_cfg kiss_fft_state;
  kiss_fft_cpx output_array[points_in_interval];
  kiss_fft_state = kiss_fftr_alloc(points_in_interval, 0, 0, 0);

  while (next_signal.size() == points_in_interval) {
    kiss_fftr(kiss_fft_state, &next_signal[0], output_array);

    double max_freq = 0;
    std::uint32_t max_freq_number = 0;
    for (int p_num = 0; p_num < points_in_interval / 2; p_num++) {
      double curr_value = output_array[p_num].r * output_array[p_num].r +
                          output_array[p_num].i * output_array[p_num].i;
      if (curr_value > max_freq) {
        max_freq = curr_value;
        max_freq_number = p_num;
      }
    }

    double freq = max_freq_number * calculated_freq_step;
    freqs.push_back(freq);

    if (cDebugMode) {
      std::cout << "freq " << freq << std::endl;
    }

    next_signal = GetNextInterval(signal);
  }

  double time_step_ms = points_in_interval * 1000.0 / fs;
  std::vector<std::pair<double, double>> freqs_with_times;
  double prev_freq = 0;
  for (auto calc_freq : freqs) {
    if (fabs(calc_freq - prev_freq) > cFreqHysteresis) {
      freqs_with_times.push_back(std::make_pair(calc_freq, time_step_ms));
    } else {
      freqs_with_times[freqs_with_times.size() - 1].second += time_step_ms;
    }

    prev_freq = calc_freq;
  }

  freqs_with_times.erase(
      std::remove_if(freqs_with_times.begin(), freqs_with_times.end(),
                     [this](std::pair<double, double> freq_with_time) {
                       return freq_with_time.second <
                              cMinimumIntervalRequiredMs;
                     }),
      freqs_with_times.end());

  return freqs_with_times;
}

std::vector<std::pair<double, double>>
FreqCalculator::GetFreqsWithIntervals(const std::vector<kiss_fft_scalar> &signal) {
  if (signal.size() < points_in_interval) {
    return std::vector<std::pair<double, double>>{};
  }

  if (cAlgorithm == Algorithm::Autocorrelation) {
    // todo
    return std::vector<std::pair<double, double>>{};
  } else if (cAlgorithm == Algorithm::FFT) {
    return GetFFTFreqWithIntervals(signal);
  } else {
    return std::vector<std::pair<double, double>>{};
  }
}
