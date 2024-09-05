/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrx-models-common v1.0 - Estruturas de dados e funções básicas compartilhadas entre os modelos
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *
 *   IDIOM: ENGLISH
 *
 *   mrx-models-common v1.0 - Data structures and basic functions shared between models
 *
 *   CONVENTIONS:

 *   CONFIGURATION:
 *       ...
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

#include <core/vector.hpp>

namespace models
{
  //-------------------------------------------------------------------------------------------------
  // Estruturas
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Posição no espaço tridimensional
   *
   * @param x Posição X
   * @param y Posição Y
   * @param z Posição Z
   */
  using Position = core::Vector3;

  /**
   * @brief Cor no espaço RGB, separadas por canal
   *
   * @param r Vermelho
   * @param g Verde
   * @param b Azul
   *
   */
  typedef struct
  {
    float r;
    float g;
    float b;
  } ColorChannels;

  /**
   * @brief Propriedades de um material
   *
   * @param diffuse Cor difusa (separa por canal)
   * @param specular Cor especular (separa por canal)
   */
  typedef struct
  {
    ColorChannels diffuse;
    ColorChannels specular;
    float shininess;
  } Material;

} // namespace models