/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxmath v1.0 - Funções matemáticas para trabalhar com Vector2, Vector3, Matrix e Quaternions
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções são sempre auto-contidas, nenhuma função usa outra função mrxmath dentro,
 *       o código necessário é re-implementado diretamente dentro.
 *          * Esta é uma maneira de evitar qualquer dependência e evitar a sobrecarga de chamadas
 *     - Os parâmetros de entrada das funções são sempre recebidos por valor
 *     - As funções sempre usam uma variável "result" para retorno
 *     - As funções sempre têm uma descrição @brief, @param e @return ESCRITAS EM PORTUGUÊS
 *          - A descrição deve estar no arquivo .cpp
 *     - Os ângulos estão sempre em radianos (macros DEG2RAD / RAD2DEG fornecidos para conveniência)
 *
 *
 *   IDIOM: ENGLISH
 *
 *   mrxmath v1.0 - Math functions to work with Vector2, Vector3, Matrix and Quaternions
 *
 *   CONVENTIONS:
 *     - Functions are always self-contained, no function use another mrxmath function inside,
 *       required code is directly re-implemented inside.
 *         - This is a way to avoid any dependency and avoid calling overhead
 *     - Functions input parameters are always received by value
 *     - Functions use always a "result" variable for return
 *     - Functions are always defined inline
 *     - Functions always have a @brief description, @param and @return tags WRITTEN IN PORTUGUESE
 *          - The description must be in the .cpp file
 *     - Angles are always in radians (DEG2RAD/RAD2DEG macros provided for convenience)
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <cmath.h>    - Required for: sinf(), cosf(), tan(), atan2f(), sqrtf(), floor(), fminf(), fmaxf(), fabs()
 *      <core/vector.hpp> - Required for: types Vector2, Vector3, Matrix and Quaternion
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *   TODO:
 *      - Implement Quaternion functions         // Implementar funções de Quaternion
 *      - Implement Vector4 functions            // Implementar funções de Vector4
 *      - Implement unit tests for all functions // Implementar testes unitários para todas as funções
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

#include <math.h>

#include <core/vector.hpp>

namespace math
{

//----------------------------------------------------------------------------------
// Defines and macros
//----------------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#ifndef DEG2RAD
#define DEG2RAD (PI / 180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f / PI)
#endif

  //----------------------------------------------------------------------------------
  // Module Functions Declaration - Utils math functions
  //----------------------------------------------------------------------------------

  float Clamp(float value, float min, float max);
  float Lerp(float start, float end, float amount);
  float Normalize(float value, float start, float end);
  float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd);
  float Wrap(float value, float min, float max);
  float FloatEquals(float a, float b);

  //----------------------------------------------------------------------------------
  // Module Functions Declaration - Vector2 math functions
  //----------------------------------------------------------------------------------

  core::Vector2 Vector2Add(core::Vector2 a, core::Vector2 b);
  core::Vector2 Vector2AddValue(core::Vector2 a, float scalar);
  core::Vector2 Vector2Subtract(core::Vector2 a, core::Vector2 b);
  core::Vector2 Vector2SubtractValue(core::Vector2 a, float scalar);
  core::Vector2 Vector2Multiply(core::Vector2 a, core::Vector2 b);
  core::Vector2 Vector2MultiplyValue(core::Vector2 a, float scalar);
  core::Vector2 Vector2Divide(core::Vector2 a, core::Vector2 b);
  core::Vector2 Vector2DivideValue(core::Vector2 a, float scalar);
  core::Vector2 Vector2Negate(core::Vector2 a);
  core::Vector2 Vector2Normalize(core::Vector2 a);
  float Vector2DotProduct(core::Vector2 a, core::Vector2 b);
  float Vector2Distance(core::Vector2 a, core::Vector2 b);
  float Vector2Angle(core::Vector2 a, core::Vector2 b);
  core::Vector2 Vector2Scale(core::Vector2 a, float scalar);
  core::Vector2 Vector2Reflect(core::Vector2 a, core::Vector2 normal);
  core::Vector2 Vector2Transform(core::Vector2 a, core::Matrix mat);

  //----------------------------------------------------------------------------------
  // Module Functions Declaration - Vector3 math functions
  //----------------------------------------------------------------------------------

  core::Vector3 Vector3Add(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3AddValue(core::Vector3 a, float scalar);
  core::Vector3 Vector3Subtract(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3SubtractValue(core::Vector3 a, float scalar);
  core::Vector3 Vector3Multiply(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3MultiplyValue(core::Vector3 a, float scalar);
  core::Vector3 Vector3Divide(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3DivideValue(core::Vector3 a, float scalar);
  core::Vector3 Vector3Negate(core::Vector3 a);
  core::Vector3 Vector3Normalize(core::Vector3 a);
  float Vector3DotProduct(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3CrossProduct(core::Vector3 a, core::Vector3 b);
  float Vector3Distance(core::Vector3 a, core::Vector3 b);
  float Vector3Angle(core::Vector3 a, core::Vector3 b);
  core::Vector3 Vector3Scale(core::Vector3 a, float scale);
  core::Vector3 Vector3Reflect(core::Vector3 a, core::Vector3 normal);
  core::Vector3 Vector3Transform(core::Vector3 a, core::Matrix mat);
  core::Vector3 Vector3Perpendicular(core::Vector3 a);
  core::Vector3 Vector3RotateByQuaternion(core::Vector3 a, core::Quaternion q);
  core::Vector3 Vector3RotateByAxisAngle(core::Vector3 a, core::Vector3 axis, float angle);

  //----------------------------------------------------------------------------------
  // Module Functions Declaration - Matrix math functions
  //----------------------------------------------------------------------------------

  core::Matrix MatrixAdd(core::Matrix a, core::Matrix b);
  core::Matrix MatrixSubtract(core::Matrix a, core::Matrix b);
  core::Matrix MatrixMultiply(core::Matrix a, core::Matrix b);
  core::Vector4 MatrixMultiplyVector(core::Matrix mat, core::Vector4 vec);
  float MatrixDeterminant(core::Matrix mat);
  core::Matrix MatrixInvert(core::Matrix mat);
  core::Matrix MatrixTranspose(core::Matrix mat);

  // Matrix transformations
  core::Matrix MatrixTranslate(core::Vector3 vec);
  core::Matrix MatrixRotate(core::Vector3 axis, float angle);
  core::Matrix MatrixScale(core::Vector3 vec);

  core::Matrix RotationMatrixX(float angle);
  core::Matrix RotationMatrixY(float angle);
  core::Matrix RotationMatrixZ(float angle);
} // namespace math
