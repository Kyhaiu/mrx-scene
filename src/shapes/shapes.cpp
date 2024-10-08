#include <shapes/shapes.hpp>

float map(float value, float start1, float stop1, float start2, float stop2)
{
  return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

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
      // Back Face
      {2, 6, 7},
      {7, 3, 2},
      // Left Face
      {0, 4, 5},
      {5, 1, 0},
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

/**
 * @brief Método para criação de uma pirâmide
 *
 * @param base Tamanho da base da pirâmide
 * @param height Altura da pirâmide
 * @return models::Mesh* Ponteiro para a malha da pirâmide
 */
models::Mesh *shapes::pyramid(float base, float height)
{

  core::Vertex *v0 = new core::Vertex(2.0f, 2.0f, 0.0f, 1.0f, nullptr, "A");
  core::Vertex *v1 = new core::Vertex(-2.0f, 2.0f, 0.0f, 1.0f, nullptr, "B");
  core::Vertex *v2 = new core::Vertex(-2.0f, -2.0f, 0.0f, 1.0f, nullptr, "C");
  core::Vertex *v3 = new core::Vertex(2.0f, -2.0f, 0.0f, 1.0f, nullptr, "D");
  core::Vertex *v4 = new core::Vertex(0.0f, 0.0f, 2.0f, 1.0f, nullptr, "E");

  std::vector<std::vector<int>> edges = {
      // Base
      {0, 3, 2, 1},
      // Frente
      {0, 4, 3},
      // Direita
      {0, 1, 4},
      // Trás
      {1, 2, 4},
      // Esquerda
      {2, 3, 4}};

  models::Mesh *pyramid = new models::Mesh({v0, v1, v2, v3, v4}, edges, "pyramid");

  return pyramid;
}

/**
 * @brief Método para criação de uma esfera simples
 *
 * @param radius Distância dos vértices da origem
 * @param rings Resolução vertical(latitude) da esfera
 * @param segments Resolução horizontal(longitude) da esfera
 * @return models::Mesh* Ponteiro para a malha da esfera
 */
models::Mesh *shapes::sphere(float radius, int rings, int segments)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  for (int latitude = 0; latitude <= rings; latitude++)
  {
    float theta = latitude * PI / rings;
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    for (int longitude = 0; longitude <= segments; longitude++)
    {
      float phi = longitude * 2.0f * PI / segments;
      float sinPhi = sin(phi);
      float cosPhi = cos(phi);

      // https://en.wikipedia.org/wiki/Spherical_coordinate_system
      // x = r * cos(phi) * sin(theta)
      // y = r * sin(phi) * sin(theta)
      // z = r * cos(theta)
      float x = radius * cosPhi * sinTheta;
      float y = radius * sinPhi * sinTheta;
      float z = radius * cosTheta;

      // Cria os vértices
      core::Vertex *vertex = new core::Vertex(x, y, z, 1.0f, nullptr, "v" + std::to_string(vertices.size()));
      vertices.push_back(vertex);
    }
  }

  // Gerar as faces triangulares (sentido anti-horário)
  for (int latitude = 0; latitude < rings; latitude++)
  {
    for (int longitude = 0; longitude < segments; longitude++)
    {
      // a------b
      // | f0 / |
      // |   /  |
      // | / f1 |
      // c------d
      // a = latitude * (segments + 1) + longitude
      // b = a + 1
      // c = a + segments + 1
      // d = c + 1

      // Caminho dos vértices
      // a -> c -> b
      // c -> d -> b
      int a = (latitude * (segments + 1)) + longitude;
      int c = a + segments + 1;
      int b = a + 1;
      int d = c + 1;

      // Cada quadrado formado é dividido em dois triângulos
      faces.push_back({a, c, b}); // Triângulo superior (f0)
      faces.push_back({c, d, b}); // Triângulo inferior (f1)
    }
  }

  models::Mesh *sphere = new models::Mesh(vertices, faces, "sphere");

  return sphere;
}

struct hash_pair
{
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const
  {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ (hash2 << 1); // Combinar os dois hashes
  }
};

// Função para encontrar ou criar um novo vértice no meio de dois vértices
int getMidPoint(int v1, int v2, std::unordered_map<std::pair<int, int>, int, hash_pair> &cache, std::vector<core::Vertex *> &vertices)
{
  auto key = std::minmax(v1, v2);
  if (cache.find(key) != cache.end())
  {
    return cache[key];
  }

  // Obter coordenadas dos dois vértices
  core::Vertex *vertex1 = vertices[v1];
  core::Vertex *vertex2 = vertices[v2];

  // Encontrar ponto médio entre v1 e v2
  core::Vertex *midVertex = new core::Vertex(
      (vertex1->getX() + vertex2->getX()) / 2.0f,
      (vertex1->getY() + vertex2->getY()) / 2.0f,
      (vertex1->getZ() + vertex2->getZ()) / 2.0f,
      1.0f, nullptr, "v" + std::to_string(vertices.size()));
  midVertex->setVector(midVertex->normalize());

  vertices.push_back(midVertex);
  cache[key] = vertices.size() - 1;

  return vertices.size() - 1;
}

/**
 * @brief Método para criação de uma icosaedro
 *
 * @param radius Distância dos vértices da origem
 * @param subdivisions Número de subdivisões em cima da malha mais básica. O número de faces quadruplica a cada subdivisão.
 * @return models::Mesh* Ponteiro para a malha do icosaedro
 */
models::Mesh *shapes::icosphere(float radius, int subdivisions)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  // Constantes do Icosaedro
  const float t = (1.0f + std::sqrtf(5.0)) / 2.0f;
  const float s = std::sqrtf(1 + t * t);

  // Definir os 12 vértices iniciais de um Icosaedro
  vertices.push_back(new core::Vertex(-1, t, 0, 1.0f, nullptr, "v0"));
  vertices.push_back(new core::Vertex(1, t, 0, 1.0f, nullptr, "v1"));
  vertices.push_back(new core::Vertex(-1, -t, 0, 1.0f, nullptr, "v2"));
  vertices.push_back(new core::Vertex(1, -t, 0, 1.0f, nullptr, "v3"));

  vertices.push_back(new core::Vertex(0, -1, t, 1.0f, nullptr, "v4"));
  vertices.push_back(new core::Vertex(0, 1, t, 1.0f, nullptr, "v5"));
  vertices.push_back(new core::Vertex(0, -1, -t, 1.0f, nullptr, "v6"));
  vertices.push_back(new core::Vertex(0, 1, -t, 1.0f, nullptr, "v7"));

  vertices.push_back(new core::Vertex(t, 0, -1, 1.0f, nullptr, "v8"));
  vertices.push_back(new core::Vertex(t, 0, 1, 1.0f, nullptr, "v9"));
  vertices.push_back(new core::Vertex(-t, 0, -1, 1.0f, nullptr, "v10"));
  vertices.push_back(new core::Vertex(-t, 0, 1, 1.0f, nullptr, "v11"));

  // Normalizar os vértices
  for (auto &vertex : vertices)
  {
    vertex->setVector(vertex->normalize());
  }

  // Definir as 20 faces triangulares do Icosaedro
  faces = {
      {0, 11, 5},
      {0, 5, 1},
      {0, 1, 7},
      {0, 7, 10},
      {0, 10, 11},
      {1, 5, 9},
      {5, 11, 4},
      {11, 10, 2},
      {10, 7, 6},
      {7, 1, 8},
      {3, 9, 4},
      {3, 4, 2},
      {3, 2, 6},
      {3, 6, 8},
      {3, 8, 9},
      {4, 9, 5},
      {2, 4, 11},
      {6, 2, 10},
      {8, 6, 7},
      {9, 8, 1}};

  // Subdivisão recursiva para aproximar uma esfera
  std::unordered_map<std::pair<int, int>, int, hash_pair> cache;
  for (int i = 0; i < subdivisions; i++)
  {
    std::vector<std::vector<int>> newFaces;
    for (auto &face : faces)
    {
      int v1 = face[0];
      int v2 = face[1];
      int v3 = face[2];

      // Encontrar pontos médios e criar novos triângulos
      int a = getMidPoint(v1, v2, cache, vertices);
      int b = getMidPoint(v2, v3, cache, vertices);
      int c = getMidPoint(v3, v1, cache, vertices);

      newFaces.push_back({v1, a, c});
      newFaces.push_back({v2, b, a});
      newFaces.push_back({v3, c, b});
      newFaces.push_back({a, b, c});
    }
    faces = newFaces;
  }

  // Ajustar os vértices para o raio desejado
  for (auto &vertex : vertices)
  {
    vertex->setX(vertex->getX() * radius);
    vertex->setY(vertex->getY() * radius);
    vertex->setZ(vertex->getZ() * radius);
  }

  // Criar a malha da Icosphere
  models::Mesh *icosphere = new models::Mesh(vertices, faces, "icosphere");

  return icosphere;
}

/**
 * @brief Método para criação de um polígono regular
 *
 * @note O polígono é achatado no plano XY
 *
 * @param sides Número de lados do polígono
 * @param radius Raio do polígono
 * @return models::Mesh* Ponteiro para a malha do polígono
 */
models::Mesh *shapes::polygon(int sides, float radius)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  // Garante que o polígono tenha pelo menos 3 lados
  if (sides < 3)
    sides = 3;

  // Ângulo entre vértices consecutivos (em radianos)
  float angleStep = 2.0f * PI / sides;

  // Gera vértices
  for (int i = 0; i < sides; i++)
  {
    float x = radius * std::cosf(i * angleStep);
    float y = radius * std::sinf(i * angleStep);

    // Cria o vértice e adicionar à lista
    core::Vertex *vertex = new core::Vertex(x, y, 0.0f, 1.0f, nullptr, "v" + std::to_string(i));
    vertices.push_back(vertex);
  }

  // Define as faces (triângulos) que conectam os vértices ao centro
  core::Vertex *center = new core::Vertex(0.0f, 0.0f, 0.0f, 1.0f, nullptr, "center");
  vertices.push_back(center);
  int centerIndex = vertices.size() - 1;

  // Gera as faces triangulares (sentido anti-horário)
  for (int i = 0; i < sides; i++)
  {
    int next = (i + 1) % sides;
    faces.push_back({i, next, centerIndex});
  }

  // Cria a malha do polígono
  models::Mesh *polygon = new models::Mesh(vertices, faces, "polygon");

  return polygon;
}

/**
 * @brief Método para criação de um cilindro
 *
 * @param radius Raio do cilindro
 * @param height Altura do cilindro
 * @param segments Número de segmentos do cilindro
 * @return models::Mesh* Ponteiro para a malha do cilindro
 *
 */
models::Mesh *shapes::cone(float radius, float height, int segments)
{
  std::vector<core::Vertex *> vertexes;
  std::vector<std::vector<int>> faces;

  for (int i = 0; i < segments; i++)
  {
    float theta = static_cast<float>(i) / segments;
    float ratio = 2.0f * PI * theta;
    float x = radius * std::cosf(ratio);
    float y = radius * std::sinf(ratio);

    core::Vertex *vertex = new core::Vertex(x, y, 0.0f, 1.0f, nullptr, "v" + std::to_string(i));
    vertexes.push_back(vertex);
  }

  // Adiciona o vértice central da base
  core::Vertex *center = new core::Vertex(0.0f, 0.0f, 0.0f, 1.0f, nullptr, "center");
  vertexes.push_back(center);
  int centerIndex = vertexes.size() - 1;

  // Cria a face da base (sentido anti-horário)
  for (int i = 0; i < segments; i++)
  {
    int next = (i + 1) % segments;
    faces.push_back({i, centerIndex, next});
  }

  // Adiciona o vértice central do topo
  core::Vertex *top = new core::Vertex(0.0f, 0.0f, height, 1.0f, nullptr, "top");
  vertexes.push_back(top);
  int topIndex = vertexes.size() - 1;

  // Conecta o topo com os vértices da base (sentido anti-horário)
  for (int i = 0; i < segments; i++)
  {
    int next = (i + 1) % segments;
    faces.push_back({i, next, topIndex});
  }

  // Cria a malha do cilindro
  models::Mesh *cone = new models::Mesh(vertexes, faces, "cone");

  return cone;
}