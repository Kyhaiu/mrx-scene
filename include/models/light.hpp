/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrx-light v1.0 - Estruturas de iluminação e funções de manipulação de iluminação
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *
 *   IDIOM: ENGLISH
 *
 *   mrx-light v1.0 - Lighting structures and lighting manipulation functions
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

#include <models/common.hpp>

#include <models/colors.hpp>
#include <math/math.hpp>

namespace models
{
  //-------------------------------------------------------------------------------------------------
  // Estruturas
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Luz Ambiente
   *
   * @param intensity Intensidade das cores da Luz Ambiente
   * @param ka Coeficiente de reflexão ambiente
   */
  typedef struct
  {
    // Intensidade da Luz Ambiente
    models::Color intensity;
    // Coeficiente de reflexão ambiente
    models::ColorChannels ka;
  } AmbientLight;

  /**
   * @brief Lampada Omnidirecional
   *
   * @param position Posição da Luz
   * @param color Cor da Luz

   A cena pode ter um vetor de lampadas omni
   */
  typedef struct
  {
    // Posição da Luz
    Position position;
    // Intensidade da Luz da Lampada
    ColorChannels color;
  } Omni;

  /**
   * @brief Luz
   *
   * @param ambient Intensidade da Luz Ambiente
   * @param color Cor da Luz
   */
  typedef struct
  {
    // Componentes da Luz Ambiente
    AmbientLight ambient;
    // Light Color
    ColorChannels color;
  } Light;

  //-------------------------------------------------------------------------------------------------
  // Funções
  //-------------------------------------------------------------------------------------------------

  void FlatShading(const models::Light &light, const models::Omni &omni, const core::Vector3 &normal, const core::Vector3 &eye, models::Color &color, const models::Material &material);

} // namespace models