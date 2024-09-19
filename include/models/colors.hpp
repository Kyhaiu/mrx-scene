/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrx-color v1.0 - Estruturas de cores e funções de manipulação de cores
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *
 *   IDIOM: ENGLISH
 *
 *   mrx-color v1.0 - Color structures and color manipulation functions
 *
 *   CONVENTIONS:

 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *     - <gui/imgui/imgui.h> - Required for: IM_COL32()
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

#include <gui/imgui/imgui.h>

#include <models/common.hpp>
#include <math/math.hpp>

namespace models
{

#define MAX_COLOR_VALUE 255
#define MIN_COLOR_VALUE 0

  // Uint8 é um tipo de dado que armazena valores inteiros de 0 a 255 (usado para cores)
  typedef unsigned char Uint8;

  // Estrutura de cor com 4 canais: R, G, B e A
  typedef struct Color
  {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color operator+(const Color &color) const
    {
      return {
          static_cast<Uint8>(r + color.r),
          static_cast<Uint8>(g + color.g),
          static_cast<Uint8>(b + color.b),
          static_cast<Uint8>(a + color.a)};
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &color)
    {
      os << "R: " << static_cast<int>(color.r) << " G: " << static_cast<int>(color.g) << " B: " << static_cast<int>(color.b) << " A: " << static_cast<int>(color.a);
      return os;
    }
  } Color;

#define MIN_COLOR_VALUE 0
#define MAX_COLOR_VALUE 255

  // Cores básicas

  // Transparente = {0, 0, 0, 0}
  const models::Color TRANSPARENT = {MIN_COLOR_VALUE, MIN_COLOR_VALUE, MIN_COLOR_VALUE, MIN_COLOR_VALUE};
  // Branco = {255, 255, 255, 255}
  const models::Color WHITE = {MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
  // Preto = {0, 0, 0, 255}
  const models::Color BLACK = {MIN_COLOR_VALUE, MIN_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE};
  // Vermelho = {255, 0, 0, 255}
  const models::Color RED = {MAX_COLOR_VALUE, MIN_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE};
  // Verde = {0, 255, 0, 255}
  const models::Color GREEN = {MIN_COLOR_VALUE, MAX_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE};
  // Azul = {0, 0, 255, 255}
  const models::Color BLUE = {MIN_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
  // Amarelo = {255, 255, 0, 255}
  const models::Color YELLOW = {MAX_COLOR_VALUE, MAX_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE};
  // Ciano = {0, 255, 255, 255}
  const models::Color CYAN = {MIN_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
  // Magenta = {255, 0, 255, 255}
  const models::Color MAGENTA = {MAX_COLOR_VALUE, MIN_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};

  // Cores cinza
  // Cinza 25% = {63, 63, 63, 255}
  const models::Color GRAY_25 = {MAX_COLOR_VALUE / 4, MAX_COLOR_VALUE / 4, MAX_COLOR_VALUE / 4, MAX_COLOR_VALUE};
  // Cinza 50% = {127, 127, 127, 255}
  const models::Color GRAY_50 = {MAX_COLOR_VALUE / 2, MAX_COLOR_VALUE / 2, MAX_COLOR_VALUE / 2, MAX_COLOR_VALUE};
  // Cinza 75% = {191, 191, 191, 255}
  const models::Color GRAY_75 = {3 * MAX_COLOR_VALUE / 4, 3 * MAX_COLOR_VALUE / 4, 3 * MAX_COLOR_VALUE / 4, MAX_COLOR_VALUE};

  /**
   * @brief Função para interpolar entre duas cores com base em um valor de interpolação t.
   *
   * @param color1 - Cor 1
   * @param color2 - Cor 2
   * @param t - Valor de interpolação (0 a 1)
   * @return constexpr Color
   *
   * @note A interpolação é linear, ou seja, a cor resultante é uma combinação linear das cores de entrada.
   */
  constexpr Color InterpolateColors(const Color &color1, const Color &color2, float t)
  {
    return {
        static_cast<Uint8>(color1.r + t * (color2.r - color1.r)),
        static_cast<Uint8>(color1.g + t * (color2.g - color1.g)),
        static_cast<Uint8>(color1.b + t * (color2.b - color1.b)),
        static_cast<Uint8>(color1.a + t * (color2.a - color1.a))};
  }

  /**
   * @brief Compara duas cores
   *
   * @param color1 Cor 1
   * @param color2 Cor 2
   * @return true - Se as cores forem iguais
   * @return false - Se as cores forem diferentes
   */
  constexpr bool CompareColors(const Color &color1, const Color &color2)
  {
    return (color1.r == color2.r) && (color1.g == color2.g) && (color1.b == color2.b) && (color1.a == color2.a);
  }

  /**
   * @brief Converte uma cor SDL para um valor unsigned int (ImU32 do ImGui)
   *
   * @param color Cor
   * @return unsigned int Valor IM_COL32
   */
  constexpr unsigned int GET_COLOR_UI32(const Color &color)
  {
    return IM_COL32(color.r, color.g, color.b, color.a);
  }

  /**
   * @brief Converte o valor models::Color para um valor models::ColorChannels
   *
   * @param color Cor
   * @return ColorChannels Canais de cor
   *
   * @note a função apenas converte os valores de Uint8 para float
   */
  constexpr ColorChannels ColorToChannels(const Color &color)
  {
    return {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)};
  }

} // namespace models