/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxcamera v1.0 - Modelo básico de benchmark
 *
 *   CONVENÇÕES:
 *
 *   IDIOM: ENGLISH
 *
 *   mrxcamera v1.0 - Basic benchmark model
 *
 *   CONVENTIONS:
 *
 *   CONFIGURATION:
 *
 *   DEPENDENCIES:
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *
 *   LICENSE: GPL 3.0
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************/
#pragma once

#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip> // Para formatar a timestamp

namespace models
{
  typedef struct Benchmark
  {
    /**
     * @brief Tempo total de execução
     *
     */
    std::chrono::duration<double> total_time;
    /**
     * @brief Tempo de início
     *
     */
    std::chrono::high_resolution_clock::time_point start_time;
    /**
     * @brief Tempo de término
     *
     */
    std::chrono::high_resolution_clock::time_point end_time;
    /**
     * @brief Número total de frames gerados durante a execução
     *
     */
    double total_frames;
    /**
     * @brief Média de frames por segundo
     *
     */
    double average_fps;
    /**
     * @brief Vetor de tempos de execução de cada frame
     *
     */
    std::vector<double> frame_times;
    /**
     * @brief Menor tempo de execução de um frame
     *
     */
    double min_frame_time;
    /**
     * @brief Maior tempo de execução de um frame
     *
     */
    double max_frame_time;
    /**
     * @brief Média de tempo de execução de um frame
     *
     */
    double average_frame_time;
    /**
     * @brief Pior 10% dos tempos de execução
     *
     */
    std::vector<double> worst_10_percentile;
  } Benchmark;

  void benchmark_start(Benchmark *benchmark);
  void benchmark_end(Benchmark *benchmark);

} // namespace models