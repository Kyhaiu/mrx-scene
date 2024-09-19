#include <shapes/shapes.hpp>

/**
 * @brief Retorna a malha de um cubo
 *
 * @note O cubo tem tamanho unitário(-1 à 1) e está centrado na origem
 *
 * @return models::Mesh* Ponteiro para a malha do cubo
 */
models::Mesh *shapes::cube()
{
  // Bottom-left-back
  core::Vertex *v0 = new core::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, nullptr, "v0");
  // Bottom-right-back
  core::Vertex *v1 = new core::Vertex(1.0f, -1.0f, -1.0f, 1.0f, nullptr, "v1");
  // Bottom-right-front
  core::Vertex *v2 = new core::Vertex(1.0f, -1.0f, 1.0f, 1.0f, nullptr, "v2");
  // Bottom-left-front
  core::Vertex *v3 = new core::Vertex(-1.0f, -1.0f, 1.0f, 1.0f, nullptr, "v3");
  // Top-left-back
  core::Vertex *v4 = new core::Vertex(-1.0f, 1.0f, -1.0f, 1.0f, nullptr, "v4");
  // Top-right-back
  core::Vertex *v5 = new core::Vertex(1.0f, 1.0f, -1.0f, 1.0f, nullptr, "v5");
  // Top-right-front
  core::Vertex *v6 = new core::Vertex(1.0f, 1.0f, 1.0f, 1.0f, nullptr, "v6");
  // Top-left-front
  core::Vertex *v7 = new core::Vertex(-1.0f, 1.0f, 1.0f, 1.0f, nullptr, "v7");

  std::vector<std::vector<int>> edges = {
      // // Back Face
      // {2, 6, 7, 3},
      // // Left Face
      // {1, 0, 4, 5},
      // // Bottom Face
      // {3, 7, 4, 0},
      // // Right Face
      // {2, 3, 0, 1},
      // // Top Face
      // {6, 2, 1, 5},
      // // Front Face
      // {6, 5, 4, 7}};
      // Back Face
      {2, 6, 7},
      {7, 3, 2},
      // Left Face
      {1, 0, 4},
      {4, 5, 1},
      // Bottom Face
      {3, 7, 4},
      {4, 0, 3},
      // Right Face
      {2, 3, 0},
      {0, 1, 2},
      // Top Face
      {6, 2, 1},
      {1, 5, 6},
      // Front Face
      {6, 5, 4},
      {4, 7, 6}};

  models::Mesh *cube = new models::Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "cube");

  return cube;
}