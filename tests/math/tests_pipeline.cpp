#include <gtest/gtest.h>
#include <iostream>
#include <math/pipeline.hpp>
#include <core/vector.hpp>
#include <vector>

class PipelineTest : public ::testing::Test
{
protected:
  // The VRP (View Reference Point) of the SRC.
  core::Vector3 vrp = {25.0f, 15.0f, 80.0f};
  // The FP (Focal Point) of the SRC.
  core::Vector3 p = {20.0f, 10.0f, 25.0f};

  // The distance from the VRP to the focal point.
  float dp = 40;

  // The window of the SRC.
  core::Vector2 min_window = {0.0f, 0.0f};
  core::Vector2 max_window = {16.0f, 12.0f};
  // The viewport of the SRC.
  core::Vector2 min_viewport = {0.0f, 0.0f};
  core::Vector2 max_viewport = {319.0f, 239.0f};

  // The point that will be projected.
  // this points form a pyramid having 5 faces.
  core::Vector4 a = {21.2f, 0.7f, 42.3f, 1.0f};
  core::Vector4 b = {34.1f, 3.4f, 27.2f, 1.0f};
  core::Vector4 c = {18.8f, 5.6f, 14.6f, 1.0f};
  core::Vector4 d = {5.9f, 2.9f, 29.7f, 1.0f};
  core::Vector4 e = {20.0f, 20.0f, 31.6f, 1.0f};

  void SetUp() override {}
};

/**
 * @brief Test case for the function sru2src.
 *
 */
TEST_F(PipelineTest, sru2src)
{
  core::Matrix result = math::pipeline_adair::sru_to_src(vrp, p);

  std::vector<std::vector<float>> expected = {
      {0.995893206467705f, 0.000000000000000f, -0.090535746042518f, -17.654470478291100f},
      {-0.008163333305850f, 0.995926663313714f, -0.089796666364351f, -7.551083307911350f},
      {0.090166963466743f, 0.090166963466743f, 0.991836598134176f, -82.953606389403800f},
      {0.000000000000000f, 0.000000000000000f, 0.000000000000000f, 1.000000000000000f}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001f);
    }
  }
};

/**
 * @brief Test case for the function projection.
 *
 */
TEST_F(PipelineTest, projection)
{
  core::Matrix result = math::pipeline_adair::projection(vrp, p, dp);

  std::vector<std::vector<float>> expected = {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, -0.025f, 0.0f}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001f);
    }
  }
}

/**
 * @brief Test case for the function src2srt with reflection.
 *
 */
TEST_F(PipelineTest, src2srt_with_reflection)
{
  core::Matrix result = math::pipeline_adair::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  std::vector<std::vector<float>> expected = {
      {19.9375f, 0.0f, 0.0f, 0.0f},
      {0.0f, -19.9166666666667f, 0.0f, 239.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001f);
    }
  }
}

/**
 * @brief Test case for the function src2srt without reflection.
 *
 */
TEST_F(PipelineTest, src2srt_without_reflection)
{
  core::Matrix result = math::pipeline_adair::src_to_srt(min_window, min_viewport, max_window, max_viewport, false);

  std::vector<std::vector<float>> expected = {
      {19.9375f, 0.0f, 0.0f, 0.0f},
      {0.0f, 19.9166666666667f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}};

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      // after the 3rd decimal place, the error is insignificant.
      EXPECT_NEAR(result(i, j), expected[i][j], 0.0001f);
    }
  }
}

TEST_F(PipelineTest, full_pipeline)
{
  // https://pt.symbolab.com/solver/step-by-step/%5Cbegin%7Bpmatrix%7D19.9375%260%260%260%5C%5C%200%26-19.9166666666667%260%26239%5C%5C%200%260%261%260%5C%5C%200%260%260%261%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D1%260%260%260%5C%5C%200%261%260%260%5C%5C%200%260%261%260%5C%5C%200%260%26-0.025%260%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D0.995893206467705%260.000000000000000%26-0.090535746042518%26-17.654470478291100%5C%5C%20-0.008163333305850%260.995926663313714%26-0.089796666364351%26-7.551083307911350%5C%5C%200.090166963466743%260.090166963466743%260.991836598134176%26-82.95360638940380%5C%5C%200%260%260%261%5Cend%7Bpmatrix%7D%5Ccdot%5Cbegin%7Bpmatrix%7D21.2%2634.1%2618.8%265.9%2620%5C%5C%200.7%263.4%265.6%262.9%2620%5C%5C%2042.3%2627.2%2614.6%2629.7%2631.6%5C%5C%201%261%261%261%261%5Cend%7Bpmatrix%7D?or=input
  core::Matrix sru_to_src_matrix = math::pipeline_adair::sru_to_src(vrp, p);
  core::Matrix projection_matrix = math::pipeline_adair::projection(vrp, p, dp);
  core::Matrix src_to_srt_matrix = math::pipeline_adair::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  core::Matrix result_matrix = math::MatrixMultiply(src_to_srt_matrix, projection_matrix);
  result_matrix = math::MatrixMultiply(result_matrix, sru_to_src_matrix);

  // Each line represent a result vertex.
  std::vector<std::vector<float>> expected = {
      {-7.40073f, 275.99312f, -5.05415f, -288.44801f, -11.91337f},
      {448.77577f, 451.39309f, 464.46087f, 461.84355f, 100.27760f},
      {-39.02426f, -52.59438f, -66.27271f, -52.70259f, -48.00489f},
      {0.97560f, 1.31485f, 1.65681f, 1.31756f, 1.20012f}};

  core::Vector4 result = {0};
  int j = 0;

  for (auto vertex : {a, b, c, d, e})
  {
    // after the 3rd decimal place, the error is insignificant.
    result = math::MatrixMultiplyVector(result_matrix, vertex);
    EXPECT_NEAR(result.x, expected[0][j], 0.0001f);
    EXPECT_NEAR(result.y, expected[1][j], 0.0001f);
    EXPECT_NEAR(result.z, expected[2][j], 0.0001f);
    EXPECT_NEAR(result.w, expected[3][j], 0.0001f);

    j++;
  }
}