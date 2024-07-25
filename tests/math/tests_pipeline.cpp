#include <gtest/gtest.h>
#include <iostream>
#include <math/pipeline.hpp>
#include <core/vector.hpp>
#include <vector>

class PipelineTest : public ::testing::Test
{
protected:
  // The VRP (View Reference Point) of the SRC.
  core::Vector3 vrp = {25, 15, 80};
  // The FP (Focal Point) of the SRC.
  core::Vector3 p = {20, 10, 25};

  // The distance from the VRP to the focal point.
  double dp = 40;

  // The window of the SRC.
  core::Vector2 min_window = {0, 0};
  core::Vector2 max_window = {16, 12};
  // The viewport of the SRC.
  core::Vector2 min_viewport = {0, 0};
  core::Vector2 max_viewport = {319, 239};

  // The point that will be projected.
  // this points form a pyramid having 5 faces.
  core::Vector4 a = {21.2, 0.7, 42.3, 1};
  core::Vector4 b = {34.1, 3.4, 27.2, 1};
  core::Vector4 c = {18.8, 5.6, 14.6, 1};
  core::Vector4 d = {5.9, 2.9, 29.7, 1};
  core::Vector4 e = {20, 20, 31.6, 1};

  void SetUp() override {}
};

/**
 * @brief Test case for the function sru2src.
 *
 */
TEST_F(PipelineTest, sru2src)
{
  core::Matrix result = math::sru_to_src(vrp, p);

  std::vector<std::vector<float>> expected = {
      {0.995893206467705, 0.000000000000000, -0.090535746042518, -17.654470478291100},
      {-0.008163333305850, 0.995926663313714, -0.089796666364351, -7.551083307911350},
      {0.090166963466743, 0.090166963466743, 0.991836598134176, -82.953606389403800},
      {0.000000000000000, 0.000000000000000, 0.000000000000000, 1.000000000000000}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001);
    }
  }
};

/**
 * @brief Test case for the function projection.
 *
 */
TEST_F(PipelineTest, projection)
{
  core::Matrix result = math::projection(vrp, p, dp);

  std::vector<std::vector<float>> expected = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, -0.025, 0}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001);
    }
  }
}

/**
 * @brief Test case for the function src2srt with reflection.
 *
 */
TEST_F(PipelineTest, src2srt_with_reflection)
{
  core::Matrix result = math::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  std::vector<std::vector<float>> expected = {
      {19.9375, 0, 0, 0},
      {0, -19.9166666666667, 0, 239},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001);
    }
  }
}

/**
 * @brief Test case for the function src2srt without reflection.
 *
 */
TEST_F(PipelineTest, src2srt_without_reflection)
{
  core::Matrix result = math::src_to_srt(min_window, min_viewport, max_window, max_viewport, false);

  std::vector<std::vector<float>> expected = {
      {19.9375, 0, 0, 0},
      {0, 19.9166666666667, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001);
    }
  }
}

TEST_F(PipelineTest, full_pipeline)
{
  // https://pt.symbolab.com/solver/step-by-step/%5Cbegin%7Bpmatrix%7D19.9375%260%260%260%5C%5C%200%26-19.9166666666667%260%26239%5C%5C%200%260%261%260%5C%5C%200%260%260%261%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D1%260%260%260%5C%5C%200%261%260%260%5C%5C%200%260%261%260%5C%5C%200%260%26-0.025%260%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D0.995893206467705%260.000000000000000%26-0.090535746042518%26-17.654470478291100%5C%5C%20-0.008163333305850%260.995926663313714%26-0.089796666364351%26-7.551083307911350%5C%5C%200.090166963466743%260.090166963466743%260.991836598134176%26-82.95360638940380%5C%5C%200%260%260%261%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D21.2%2634.1%2618.8%265.9%2620%5C%5C%200.7%263.4%265.6%262.9%2620%5C%5C%2042.3%2627.2%2614.6%2629.7%2631.6%5C%5C%201%261%261%261%261%5Cend%7Bpmatrix%7D?or=input
  core::Matrix sru_to_src_matrix = math::sru_to_src(vrp, p);
  core::Matrix projection_matrix = math::projection(vrp, p, dp);
  core::Matrix src_to_srt_matrix = math::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  core::Matrix result_matrix = math::MatrixMultiply(src_to_srt_matrix, projection_matrix);
  result_matrix = math::MatrixMultiply(result_matrix, sru_to_src_matrix);

  // Each line represent a result vertex.
  std::vector<std::vector<float>> expected = {
      {-7.40073, 275.99312, -5.05415, -288.44801, -11.91337},
      {448.77577, 451.39309, 464.46087, 461.84355, 100.27760},
      {-39.02426, -52.59438, -66.27271, -52.70259, -48.00489},
      {0.97560, 1.31485, 1.65681, 1.31756, 1.20012}};

  core::Vector4 result = {0};
  int j = 0;

  for (auto vertex : {a, b, c, d, e})
  {
    // after the 3rd decimal place, the error is insignificant.
    result = math::MatrixMultiplyVector(result_matrix, vertex);
    EXPECT_NEAR(result.x, expected[0][j], 0.0001);
    EXPECT_NEAR(result.y, expected[1][j], 0.0001);
    EXPECT_NEAR(result.z, expected[2][j], 0.0001);
    EXPECT_NEAR(result.w, expected[3][j], 0.0001);

    j++;
  }
}