/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxvector v1.0 - declarações de tipos e funções básicas para Vector2, Vector3, Vector4, Matrix e Quaternions
 *
 *   CONVENTIONS: (Convenções)
 *     - A estrutura Matrix é definida como row-major (layout de memória) mas os nomes dos parâmetros
 *       E todas as operações matemáticas realizadas pela biblioteca consideram a estrutura como se fosse
 *       column-major. É como se versões transpostas das matrizes fossem usadas para toda a matemática
 *       Isso beneficia algumas funções tornando-as cache-friendly e também evita matrizes transpostas
 *       às vezes. Isto é apenas uma camada de abstração.
 *       Exemplo: Na ordem da memória, row0 é [m0 m4 m8 m12] mas na matemática semântica row0 é [m0 m1 m2 m3]
 *     - As funções são sempre auto-contidas, nenhuma função usa outra função dentro,
 *       sendo necessário a re-implementado diretamente dentro.
 *          * Esta é uma maneira de evitar qualquer dependência e evitar a sobrecarga de chamadas
 *     - Os parâmetros de entrada das funções são sempre recebidos por valor
 *     - As funções sempre usam uma variável "result" para retorno
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *
 *
 *   IDIOM: ENGLISH
 *
 *   mrxvector v1.0 - declarations of types and basic functions to work with Vector2, Vector3, Vector4, Matrix and Quaternions
 *
 *   CONVENTIONS:
 *     - The Matrix structure is defined as row-major but the parameter names
 *       and all mathematical operations performed by the library consider the structure as if it were
 *       column
 *       major. It is as if transposed versions of the matrices were used for all mathematics
 *       This benefits some functions making them cache-friendly
 *       and also avoids transposed matrices sometimes. This is just a layer of abstraction.
 *       Example: In memory order, row0 is [m0 m4 m8 m12] but in semantic mathematics row0 is [m0 m1 m2 m3]
 *     - Functions are always self-contained, no function uses another function inside,
 *       being necessary to re-implement directly inside.
 *          * This is a way to avoid any dependency and avoid the overhead of calls
 *     - The input parameters of the functions are always received by value
 *     - Functions always use a "result" variable for return
 *     - Functions always have a @brief, @param and @return description in the .cpp file
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <cmath> - Required for: sqrtf(), fminf(), fmaxf(), fabs()
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *   TODO:
 *      - Implement Quaternion functions         // Implementar funções de Quaternion
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

#include <core/common.hpp>

#include <utils/nlohmann/json.hpp>

#include <cmath>
#include <iostream>

using json = nlohmann::json;

namespace core
{
  // ---------------------------------------------------------------------------------
  // Types and Structures Definition
  // ---------------------------------------------------------------------------------

  typedef struct Vector2
  {
    float x;
    float y;

    json to_json()
    {
      return json{{"x", x}, {"y", y}};
    }
  } Vector2;

  typedef struct Vector3
  {
    float x;
    float y;
    float z;

    bool operator==(const Vector3 &v) const
    {
      return x == v.x && y == v.y && z == v.z;
    }

    json to_json()
    {
      return json{{"x", x}, {"y", y}, {"z", z}};
    }
  } Vector3;

  std::ostream &operator<<(std::ostream &os, const Vector3 &v);

  typedef struct Vector4
  {
    float x;
    float y;
    float z;
    float w;

    // Vector4toVector3 - Returns a Vector3 from a Vector4
    Vector3 toVector3()
    {
      return {x, y, z};
    }

    // operator << - Overloads the << operator to print the Vector4
    friend std::ostream &operator<<(std::ostream &os, const Vector4 &v)
    {
      os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
      return os;
    }

    json to_json()
    {
      return json{{"x", x}, {"y", y}, {"z", z}, {"w", w}};
    }
  } Vector4;

  typedef struct Matrix
  {
    float m0, m4, m8, m12;
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;

    float &operator()(int i, int j)
    {
      return ((float *)this)[i + j * 4];
    }

    const float &operator()(int i, int j) const
    {
      return ((float *)this)[i + j * 4];
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m)
    {
      os << m.m0 << "\t" << m.m4 << "\t" << m.m8 << "\t" << m.m12 << std::endl;
      os << m.m1 << "\t" << m.m5 << "\t" << m.m9 << "\t" << m.m13 << std::endl;
      os << m.m2 << "\t" << m.m6 << "\t" << m.m10 << "\t" << m.m14 << std::endl;
      os << m.m3 << "\t" << m.m7 << "\t" << m.m11 << "\t" << m.m15 << std::endl;
      return os;
    }

    json to_json()
    {
      return json{{"m0", m0}, {"m4", m4}, {"m8", m8}, {"m12", m12}, {"m1", m1}, {"m5", m5}, {"m9", m9}, {"m13", m13}, {"m2", m2}, {"m6", m6}, {"m10", m10}, {"m14", m14}, {"m3", m3}, {"m7", m7}, {"m11", m11}, {"m15", m15}};
    }
  } Matrix;

  typedef struct float2
  {
    float v[2];
  } float2;

  typedef struct float3
  {
    float v[3];
  } float3;

  typedef struct float4
  {
    float v[4];
  } float4;

  typedef struct float16
  {
    float v[16];
  } float16;

  typedef Vector4 Quaternion;

  // #define MRX_VECTOR2_TYPE Vector2
  // #define MRX_VECTOR3_TYPE Vector3
  // #define MRX_VECTOR4_TYPE Vector4
  // #define MRX_QUATERNION_TYPE Quaternion
  // #define MRX_MATRIX_TYPE Matrix

  //-----------------------------------------------------------------------------------
  // Module Functions Declaration
  //-----------------------------------------------------------------------------------

  Vector2 Vector2Zero();
  Vector2 Vector2One(void);
  float Vector2Length(const Vector2 v);
  bool Vector2Equals(const Vector2 v1, const Vector2 v2);
  float2 Vector2ToFloat2(const Vector2 v);
  Vector2 Vector2Lerp(const Vector2 v1, const Vector2 v2, const float amount);
  Vector2 Vector2Clamp(const Vector2 v, const Vector2 min, const Vector2 max);
  Vector2 Vector2ClampValue(const Vector2 v, const float min, const float max);

  Vector3 Vector3Zero(void);
  Vector3 Vector3One(void);
  float Vector3Length(const Vector3 v);
  bool Vector3Equals(const Vector3 v1, const Vector3 v2);
  float3 Vector3ToFloat3(const Vector3 v);
  Vector3 Vector3Lerp(const Vector3 v1, const Vector3 v2, const float amount);
  Vector3 Vector3Clamp(const Vector3 v, const Vector3 min, const Vector3 max);
  Vector3 Vector3ClampValue(const Vector3 v, const float min, const float max);

  Vector4 Vector4Zero(void);
  Vector4 Vector4One(void);
  float Vector4Length(const Vector4 v);
  bool Vector4Equals(const Vector4 v1, const Vector4 v2);
  float4 Vector4ToFloat4(const Vector4 v);
  Vector4 Vector4Lerp(const Vector4 v1, const Vector4 v2, const float amount);
  Vector4 Vector4Clamp(const Vector4 v, const Vector4 min, const Vector4 max);
  Vector4 Vector4ClampValue(const Vector4 v, const float min, const float max);

  Matrix Flota16ToMatrix(const float16 f);
  Matrix MatrixIdentity(void);
  Matrix MatrixTranspose(Matrix mat);
  float16 MatrixToFloat16(const Matrix m);
}