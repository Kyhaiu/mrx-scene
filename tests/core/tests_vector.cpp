#include <gtest/gtest.h>
#include <core/vector.hpp>

/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxvectors v1.0 - Testes unitários para o módulo e funções Vector2, Vector3, Matrix e Quaternions
 *
 *   CONVENTIONS: (Convenções)
 *    - A descrição dos testes deve ser feito em inglês
 *    - O nome do teste deve ser escrito em inglês
 *    - O nome do teste deve ser o nome da função que está sendo testada seguido de um underscore
 *      e o nome do comportamento que está sendo testado.
 *    - O nome do teste deve ser auto-explicativo
 *    - Cada teste deve apenas testar apenas um comportamento por vez.
 *      > Se a função tiver muitos casos de teste, o teste deve ser dividido em sub-testes
 *
 *
 *   IDIOM: ENGLISH
 *
 *   mrxmath v1.0 - Math functions to work with Vector2, Vector3, Matrix and Quaternions
 *
 *   CONVENTIONS:
 *    - Test description must be done in English
 *    - Test name must be written in English
 *    - Test name must be the name of the function being tested followed by an underscore
 *      and the name of the behavior being tested.
 *    - Test name must be self-explanatory
 *    - Each test must only test one behavior at a time.
 *      > If the function has many test cases, the test must be divided into sub-tests
 *
 *   CONFIGURATION:
 *       ...
 *
 *   TESTS:
 *      <core/vector.hpp>
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
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

/**
 * @brief Setup class for the Vector tests.
 *
 */
class VectorTest : public ::testing::Test
{
protected:
  core::Vector2 vector2 = {1.0, 1.0};
  core::Vector3 vector3 = {1.0, 1.0, 1.0};
  core::Vector4 vector4 = {1.0, 1.0, 1.0, 1.0};
  core::Quaternion quaternion = {1.0, 1.0, 1.0, 1.0};

  core::Matrix matrix = {1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0};

  void SetUp() override {}
};

/**
 * @brief Test case for the default values declared in VectorTest.
 *
 */
TEST_F(VectorTest, default_values)
{
  float expected_value = 1.0f;

  EXPECT_EQ(vector2.x, expected_value);
  EXPECT_EQ(vector2.y, expected_value);

  EXPECT_EQ(vector3.x, expected_value);
  EXPECT_EQ(vector3.y, expected_value);
  EXPECT_EQ(vector3.z, expected_value);

  EXPECT_EQ(vector4.x, expected_value);
  EXPECT_EQ(vector4.y, expected_value);
  EXPECT_EQ(vector4.z, expected_value);
  EXPECT_EQ(vector4.w, expected_value);

  EXPECT_EQ(quaternion.x, expected_value);
  EXPECT_EQ(quaternion.y, expected_value);
  EXPECT_EQ(quaternion.z, expected_value);
  EXPECT_EQ(quaternion.w, expected_value);

  EXPECT_EQ(matrix.m0, expected_value);
  EXPECT_EQ(matrix.m1, 0.0f);
  EXPECT_EQ(matrix.m2, 0.0f);
  EXPECT_EQ(matrix.m3, 0.0f);
  EXPECT_EQ(matrix.m4, 0.0f);
  EXPECT_EQ(matrix.m5, expected_value);
  EXPECT_EQ(matrix.m6, 0.0f);
  EXPECT_EQ(matrix.m7, 0.0f);
  EXPECT_EQ(matrix.m8, 0.0f);
  EXPECT_EQ(matrix.m9, 0.0f);
  EXPECT_EQ(matrix.m10, expected_value);
  EXPECT_EQ(matrix.m11, 0.0f);
  EXPECT_EQ(matrix.m12, 0.0f);
  EXPECT_EQ(matrix.m13, 0.0f);
  EXPECT_EQ(matrix.m14, 0.0f);
  EXPECT_EQ(matrix.m15, expected_value);
}

/**
 * @brief Test case for the zero initialization of a Vector2.
 *
 */
TEST_F(VectorTest, zero_vector2)
{
  // Arrange
  core::Vector2 expected_vector2 = {0.0, 0.0};

  // Act
  core::Vector2 actual_vector2 = core::Vector2Zero();

  // Expect
  EXPECT_EQ(actual_vector2.x, expected_vector2.x);
  EXPECT_EQ(actual_vector2.y, expected_vector2.y);
}

/**
 * @brief Test case for the zero initialization of a Vector3.
 *
 */
TEST_F(VectorTest, zero_vector3)
{
  // Arrange
  core::Vector3 expected_vector3 = {0.0, 0.0, 0.0};

  // Act
  core::Vector3 actual_vector3 = core::Vector3Zero();

  // Expect
  EXPECT_EQ(actual_vector3.x, expected_vector3.x);
  EXPECT_EQ(actual_vector3.y, expected_vector3.y);
  EXPECT_EQ(actual_vector3.z, expected_vector3.z);
}

/**
 * @brief Test case for the zero initialization of a Vector4.
 *
 */
TEST_F(VectorTest, zero_vector4)
{
  // Arrange
  core::Vector4 expected_vector4 = {0.0, 0.0, 0.0, 0.0};

  // Act
  core::Vector4 actual_vector4 = core::Vector4Zero();

  // Expect
  EXPECT_EQ(actual_vector4.x, expected_vector4.x);
  EXPECT_EQ(actual_vector4.y, expected_vector4.y);
  EXPECT_EQ(actual_vector4.z, expected_vector4.z);
  EXPECT_EQ(actual_vector4.w, expected_vector4.w);
}

/**
 * @brief Test case for the one initialization of a Vector2.
 *
 */
TEST_F(VectorTest, one_vector2)
{
  // Arrange
  core::Vector2 expected_vector2 = {1.0, 1.0};

  // Act
  core::Vector2 actual_vector2 = core::Vector2One();

  // Expect
  EXPECT_EQ(actual_vector2.x, expected_vector2.x);
  EXPECT_EQ(actual_vector2.y, expected_vector2.y);
}

/**
 * @brief Test case for the one initialization of a Vector3.
 *
 */
TEST_F(VectorTest, one_vector3)
{
  // Arrange
  core::Vector3 expected_vector3 = {1.0, 1.0, 1.0};

  // Act
  core::Vector3 actual_vector3 = core::Vector3One();

  // Expect
  EXPECT_EQ(actual_vector3.x, expected_vector3.x);
  EXPECT_EQ(actual_vector3.y, expected_vector3.y);
  EXPECT_EQ(actual_vector3.z, expected_vector3.z);
}

/**
 * @brief Test case for the one initialization of a Vector4.
 *
 */
TEST_F(VectorTest, one_vector4)
{
  // Arrange
  core::Vector4 expected_vector4 = {1.0, 1.0, 1.0, 1.0};

  // Act
  core::Vector4 actual_vector4 = core::Vector4One();

  // Expect
  EXPECT_EQ(actual_vector4.x, expected_vector4.x);
  EXPECT_EQ(actual_vector4.y, expected_vector4.y);
  EXPECT_EQ(actual_vector4.z, expected_vector4.z);
  EXPECT_EQ(actual_vector4.w, expected_vector4.w);
}

/**
 * @brief Test case for the identity initialization of a Matrix.
 *
 */
TEST_F(VectorTest, identity_matrix)
{
  // Arrange
  core::Matrix expected_matrix = {1.0, 0.0, 0.0, 0.0,
                                  0.0, 1.0, 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0};

  // Act
  core::Matrix actual_matrix = core::MatrixIdentity();

  // Expect
  EXPECT_EQ(actual_matrix.m0, expected_matrix.m0);
  EXPECT_EQ(actual_matrix.m1, expected_matrix.m1);
  EXPECT_EQ(actual_matrix.m2, expected_matrix.m2);
  EXPECT_EQ(actual_matrix.m3, expected_matrix.m3);
  EXPECT_EQ(actual_matrix.m4, expected_matrix.m4);
  EXPECT_EQ(actual_matrix.m5, expected_matrix.m5);
  EXPECT_EQ(actual_matrix.m6, expected_matrix.m6);
  EXPECT_EQ(actual_matrix.m7, expected_matrix.m7);
  EXPECT_EQ(actual_matrix.m8, expected_matrix.m8);
  EXPECT_EQ(actual_matrix.m9, expected_matrix.m9);
  EXPECT_EQ(actual_matrix.m10, expected_matrix.m10);
  EXPECT_EQ(actual_matrix.m11, expected_matrix.m11);
  EXPECT_EQ(actual_matrix.m12, expected_matrix.m12);
  EXPECT_EQ(actual_matrix.m13, expected_matrix.m13);
  EXPECT_EQ(actual_matrix.m14, expected_matrix.m14);
  EXPECT_EQ(actual_matrix.m15, expected_matrix.m15);
}

/**
 * @brief Test case for the length of a Vector2.
 *
 */
TEST_F(VectorTest, length_vector2)
{
  // In we use the vector2 variable declared in the setup

  // Arrange
  float expected_length = 1.41421356f;

  // Act
  float actual_length = core::Vector2Length(vector2);

  // Expect
  EXPECT_FLOAT_EQ(actual_length, expected_length);
}

/**
 * @brief Test case for the length of a Vector3.
 *
 */
TEST_F(VectorTest, length_vector3)
{
  // In we use the vector3 variable declared in the setup

  // Arrange
  float expected_length = 1.73205081f;

  // Act
  float actual_length = core::Vector3Length(vector3);

  // Expect
  EXPECT_FLOAT_EQ(actual_length, expected_length);
}

/**
 * @brief Test case for the length of a Vector4.
 *
 */
TEST_F(VectorTest, length_vector4)
{
  // In we use the vector4 variable declared in the setup

  // Arrange
  float expected_length = 2.0f;

  // Act
  float actual_length = core::Vector4Length(vector4);

  // Expect
  EXPECT_FLOAT_EQ(actual_length, expected_length);
}

/**
 * @brief Teste case for the function Vector2Equals.
 *
 */
TEST_F(VectorTest, equals_vector2)
{
  // In we use the vector2 variable declared in the setup

  // Arrange
  core::Vector2 vector2_2 = {1.0, 1.0};
  core::Vector2 vector2_3 = {1.0, 2.0};

  // Act
  bool actual_equality = core::Vector2Equals(vector2, vector2_2);

  // Expect
  EXPECT_TRUE(actual_equality);

  // Act
  actual_equality = core::Vector2Equals(vector2, vector2_3);

  // Expect
  EXPECT_FALSE(actual_equality);
}

/**
 * @brief Teste case for the function Vector3Equals.
 *
 */
TEST_F(VectorTest, equals_vector3)
{
  // In we use the vector3 variable declared in the setup

  // Arrange
  core::Vector3 vector3_2 = {1.0, 1.0, 1.0};
  core::Vector3 vector3_3 = {1.0, 2.0, 1.0};

  // Act
  bool actual_equality = core::Vector3Equals(vector3, vector3_2);

  // Expect
  EXPECT_TRUE(actual_equality);

  // Act
  actual_equality = core::Vector3Equals(vector3, vector3_3);

  // Expect
  EXPECT_FALSE(actual_equality);
}

/**
 * @brief Teste case for the function Vector4Equals.
 *
 */
TEST_F(VectorTest, equals_vector4)
{
  // In we use the vector4 variable declared in the setup

  // Arrange
  core::Vector4 vector4_2 = {1.0, 1.0, 1.0, 1.0};
  core::Vector4 vector4_3 = {1.0, 2.0, 1.0, 1.0};

  // Act
  bool actual_equality = core::Vector4Equals(vector4, vector4_2);

  // Expect
  EXPECT_TRUE(actual_equality);

  // Act
  actual_equality = core::Vector4Equals(vector4, vector4_3);

  // Expect
  EXPECT_FALSE(actual_equality);
}

/**
 * @brief Teste case for the function Vector2ToFloat2.
 *
 */
TEST_F(VectorTest, to_float2_vector2)
{
  // In we use the vector2 variable declared in the setup

  // Act
  core::float2 actual_float2 = core::Vector2ToFloat2(vector2);

  // Expect
  EXPECT_EQ(actual_float2.v[0], vector2.x);
  EXPECT_EQ(actual_float2.v[1], vector2.y);
}

/**
 * @brief Teste case for the function Vector3ToFloat3.
 *
 */
TEST_F(VectorTest, to_float3_vector3)
{
  // In we use the vector3 variable declared in the setup

  // Act
  core::float3 actual_float3 = core::Vector3ToFloat3(vector3);

  // Expect
  EXPECT_EQ(actual_float3.v[0], vector3.x);
  EXPECT_EQ(actual_float3.v[1], vector3.y);
  EXPECT_EQ(actual_float3.v[2], vector3.z);
}

/**
 * @brief Teste case for the function Vector4ToFloat4.
 *
 */
TEST_F(VectorTest, to_float4_vector4)
{
  // In we use the vector4 variable declared in the setup

  // Act
  core::float4 actual_float4 = core::Vector4ToFloat4(vector4);

  // Expect
  EXPECT_EQ(actual_float4.v[0], vector4.x);
  EXPECT_EQ(actual_float4.v[1], vector4.y);
  EXPECT_EQ(actual_float4.v[2], vector4.z);
  EXPECT_EQ(actual_float4.v[3], vector4.w);
}