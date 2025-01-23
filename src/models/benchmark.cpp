#include <models/benchmark.hpp>

namespace models
{
  void models::benchmark_start(Benchmark *benchmark)
  {
    benchmark->start_time = std::chrono::high_resolution_clock::now();
    benchmark->total_frames = 0.0;
    benchmark->average_fps = 0.0;
    benchmark->frame_times.clear();
    benchmark->min_frame_time = 0.0;
    benchmark->max_frame_time = 0.0;
    benchmark->average_frame_time = 0.0;
    benchmark->worst_10_percentile.clear();
  }

  void models::benchmark_end(Benchmark *benchmark)
  {
    benchmark->end_time = std::chrono::high_resolution_clock::now();
    benchmark->total_time = benchmark->end_time - benchmark->start_time;
  }
} // namespace models