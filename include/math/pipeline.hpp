/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxpipeline v1.0 - Processos e funções para trabalhar com o Pipeline de Visualização 3D
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções são sempre auto-contidas, nenhuma função usa outra função mrxpipeline dentro,
 *       o código necessário é re-implementado diretamente dentro.
 *          * Esta é uma maneira de evitar qualquer dependência e evitar a sobrecarga de chamadas
 *     - Os parâmetros de entrada das funções são sempre recebidos por valor
 *     - As funções sempre usam uma variável "result" para retorno
 *     - As funções sempre têm uma descrição @brief, @param e @return ESCRITAS EM PORTUGUÊS
 *          - A descrição deve estar no arquivo .cpp
 *
 *   IDIOM: ENGLISH
 *
 *   mrxpipeline v1.0 - Processes and functions to work with the 3D Visualization Pipeline
 *
 *   CONVENTIONS:
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *   TODO:
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

#include <math/math.hpp>
#include <core/vector.hpp>

namespace math
{
  core::Matrix sru_to_src(const core::Vector3 &vrp, const core::Vector3 fp);
  core::Matrix projection(const core::Vector3 &vrp, const core::Vector3 p, const float d);
  core::Matrix src_to_srt(const core::Vector2 min_window, const core::Vector2 min_viewport, const core::Vector2 max_window, const core::Vector2 max_viewport, bool reflected);
} // namespace math