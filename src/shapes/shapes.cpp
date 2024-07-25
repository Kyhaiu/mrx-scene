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
  core::Vertex *v0 = new core::Vertex(-1, -1, -1, 1, nullptr, "v0");
  core::Vertex *v1 = new core::Vertex(1, -1, -1, 1, nullptr, "v1");
  core::Vertex *v2 = new core::Vertex(1, -1, 1, 1, nullptr, "v2");
  core::Vertex *v3 = new core::Vertex(-1, -1, 1, 1, nullptr, "v3");
  core::Vertex *v4 = new core::Vertex(-1, 1, -1, 1, nullptr, "v4");
  core::Vertex *v5 = new core::Vertex(1, 1, -1, 1, nullptr, "v5");
  core::Vertex *v6 = new core::Vertex(1, 1, 1, 1, nullptr, "v6");
  core::Vertex *v7 = new core::Vertex(-1, 1, 1, 1, nullptr, "v7");

  std::vector<std::vector<int>> edges = {
      {2, 6, 7, 3},
      {0, 4, 5, 1},
      {3, 7, 4, 0},
      {0, 1, 2, 3},
      {2, 1, 5, 6},
      {6, 5, 4, 7}};

  models::Mesh *cube = new models::Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "cube");

  return cube;
}