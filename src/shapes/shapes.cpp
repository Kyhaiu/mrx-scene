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
models::Mesh *shapes::cube(core::Vector3 shift)
{
  // Vértices com deslocamento aplicado corretamente
  core::Vertex *v0 = new core::Vertex(-1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v0");
  core::Vertex *v1 = new core::Vertex(1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v1");
  core::Vertex *v2 = new core::Vertex(1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v2");
  core::Vertex *v3 = new core::Vertex(-1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v3");
  core::Vertex *v4 = new core::Vertex(-1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v4");
  core::Vertex *v5 = new core::Vertex(1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v5");
  core::Vertex *v6 = new core::Vertex(1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v6");
  core::Vertex *v7 = new core::Vertex(-1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v7");

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
 * @brief Retorna a malha de um volume canônico
 *
 * @param z_min Distância do plano de projeção próximo
 *
 * @note O volume canônico tem tamanho unitário(-1 à 1, exceção z) e está centrado na origem
 *
 * @return models::Mesh* Ponteiro para a malha do volume canônico
 * @note Usado no recorte 3d
 */
models::Mesh *shapes::canonical_volume(float z_min)
{

  // Bottom-left-back
  core::Vertex *v0 = new core::Vertex(-1.0f, -1.0f, 0.0f, 1.0f, nullptr, "v0");
  // Bottom-right-back
  core::Vertex *v1 = new core::Vertex(1.0f, -1.0f, 0.0f, 1.0f, nullptr, "v1");
  // Bottom-right-front
  core::Vertex *v2 = new core::Vertex(1.0f, -1.0f, 1.0f, 1.0f, nullptr, "v2");
  // Bottom-left-front
  core::Vertex *v3 = new core::Vertex(-1.0f, -1.0f, 1.0f, 1.0f, nullptr, "v3");
  // Top-left-back
  core::Vertex *v4 = new core::Vertex(-1.0f, 1.0f, 0.0f, 1.0f, nullptr, "v4");
  // Top-right-back
  core::Vertex *v5 = new core::Vertex(1.0f, 1.0f, 0.0f, 1.0f, nullptr, "v5");
  // Top-right-front
  core::Vertex *v6 = new core::Vertex(1.0f, 1.0f, 1.0f, 1.0f, nullptr, "v6");
  // Top-left-front
  core::Vertex *v7 = new core::Vertex(-1.0f, 1.0f, 1.0f, 1.0f, nullptr, "v7");

  std::vector<std::vector<int>> edges = {
      // Back Face
      {2, 6, 7, 3},
      // Left Face
      {1, 0, 4, 5},
      // Bottom Face
      {3, 7, 4, 0},
      // Right Face
      {2, 3, 0, 1},
      // Top Face
      {6, 2, 1, 5},
      // Front Face
      {6, 5, 4, 7}};

  models::Mesh *cube = new models::Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "canonical_volume");

  return cube;
}

/**
 * @brief Método para criação de uma pirâmide
 *
 * @param base Tamanho da base da pirâmide
 * @param height Altura da pirâmide
 * @param shift Deslocamento da pirâmide
 * @return models::Mesh* Ponteiro para a malha da pirâmide
 */
models::Mesh *shapes::pyramid(float base, float height, core::Vector3 shift)
{

  core::Vertex *v0 = new core::Vertex(2.0f + shift.x, 2.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "A");
  core::Vertex *v1 = new core::Vertex(-2.0f + shift.x, 2.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "B");
  core::Vertex *v2 = new core::Vertex(-2.0f + shift.x, -2.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "C");
  core::Vertex *v3 = new core::Vertex(2.0f + shift.x, -2.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "D");
  core::Vertex *v4 = new core::Vertex(0.0f + shift.x, 0.0f + shift.y, 2.0f + shift.z, 1.0f, nullptr, "E");

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

  // Vértice do polo norte
  vertices.push_back(new core::Vertex(0.0f, 0.0f, radius, 1.0f, nullptr, "v0"));

  // Vértices dos anéis intermediários
  for (int latitude = 1; latitude < rings; latitude++)
  {
    float theta = latitude * PI / rings;
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    for (int longitude = 0; longitude <= segments; longitude++)
    {
      float phi = longitude * 2.0f * PI / segments;
      float sinPhi = sin(phi);
      float cosPhi = cos(phi);

      float x = radius * cosPhi * sinTheta;
      float y = radius * sinPhi * sinTheta;
      float z = radius * cosTheta;

      core::Vertex *vertex = new core::Vertex(x, y, z, 1.0f, nullptr, "v" + std::to_string(vertices.size()));
      vertices.push_back(vertex);
    }
  }

  // Vértice do polo sul
  vertices.push_back(new core::Vertex(0.0f, 0.0f, -radius, 1.0f, nullptr, "v" + std::to_string(vertices.size())));

  // Topo (triângulos conectando o polo norte ao primeiro anel)
  int firstRingStart = 1;
  for (int longitude = 0; longitude < segments; longitude++)
  {
    int a = 0; // Polo norte
    int b = firstRingStart + longitude;
    int c = firstRingStart + longitude + 1;
    faces.push_back({a, b, c});
  }

  // Anéis intermediários (quads divididos em dois triângulos)
  for (int latitude = 0; latitude < rings - 2; latitude++)
  {
    int currentRingStart = firstRingStart + latitude * (segments + 1);
    int nextRingStart = currentRingStart + (segments + 1);

    for (int longitude = 0; longitude < segments; longitude++)
    {
      int a = currentRingStart + longitude;
      int b = a + 1;
      int c = nextRingStart + longitude;
      int d = c + 1;

      faces.push_back({a, b, c});
      faces.push_back({b, d, c});
    }
  }

  // Base (triângulos conectando o último anel ao polo sul)
  int lastRingStart = firstRingStart + (rings - 2) * (segments + 1);
  int southPole = vertices.size() - 1;
  for (int longitude = 0; longitude < segments; longitude++)
  {
    int a = lastRingStart + longitude;
    int b = lastRingStart + longitude + 1;
    int c = southPole;
    faces.push_back({a, b, c});
  }

  return new models::Mesh(vertices, faces, "sphere");
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

/**
 * @brief Calcula o ponto médio entre dois vértices e gerencia o cache.
 *
 * Esta função calcula as coordenadas do ponto médio entre dois vértices,
 * dados seus índices no vetor de vértices. Ela utiliza um cache para evitar
 * o recálculo de pontos médios já computados, otimizando o processo de
 * subdivisão de uma malha.
 *
 * @param v1 Índice do primeiro vértice.
 * @param v2 Índice do segundo vértice.
 * @param cache Referência para um mapa que armazena os pontos médios calculados.
 *              A chave do mapa é um par ordenado dos índices dos vértices
 *              (usando std::minmax para garantir que a ordem não importe), e o
 *              valor é o índice do novo vértice na lista `vertices`.
 * @param vertices Referência para um vetor que armazena todos os vértices da malha.
 *
 * @return O índice do vértice do ponto médio no vetor `vertices`.
 *         Se o ponto médio já tiver sido calculado, retorna o índice
 *         armazenado no cache. Caso contrário, calcula o ponto médio,
 *         adiciona o novo vértice ao vetor `vertices` e atualiza o cache
 *         antes de retornar o índice.
 *
 * @remark A função normaliza o vetor do ponto médio antes de adicioná-lo
 *         ao vetor de vértices, garantindo que o novo vértice fique na
 *         superfície de uma esfera unitária (ou seja, a distância da origem
 *         é 1).
 *
 * @remark O cache é crucial para o desempenho, pois evita cálculos
 *         redundantes do mesmo ponto médio, o que seria computacionalmente
 *         custoso, especialmente com um grande número de subdivisões.
 *
 * @code
 * // Exemplo de uso:
 * std::vector<core::Vertex*> vertices;
 * std::unordered_map<std::pair<int, int>, int, hash_pair> cache;
 * // ... (adicionar vértices iniciais ao vetor vertices) ...
 * int indexMidPoint = getMidPoint(0, 1, cache, vertices);
 * core::Vertex* midVertex = vertices[indexMidPoint];
 * // ... (usar o vértice midVertex) ...
 * @endcode
 */
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
 * @param shift Deslocamento do icosaedro
 * @return models::Mesh* Ponteiro para a malha do icosaedro
 */
models::Mesh *shapes::icosphere(float radius, int subdivisions, core::Vector3 shift)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  // Constantes do Icosaedro
  const float t = (1.0f + std::sqrtf(5.0)) / 2.0f;

  // Definir os 12 vértices iniciais de um Icosaedro
  vertices.push_back(new core::Vertex(-1 + shift.x, t + shift.y, 0 + shift.z, 1.0f, nullptr, "v0"));
  vertices.push_back(new core::Vertex(1 + shift.x, t + shift.y, 0 + shift.z, 1.0f, nullptr, "v1"));
  vertices.push_back(new core::Vertex(-1 + shift.x, -t + shift.y, 0 + shift.z, 1.0f, nullptr, "v2"));
  vertices.push_back(new core::Vertex(1 + shift.x, -t + shift.y, 0 + shift.z, 1.0f, nullptr, "v3"));

  vertices.push_back(new core::Vertex(0 + shift.x, -1 + shift.y, t + shift.z, 1.0f, nullptr, "v4"));
  vertices.push_back(new core::Vertex(0 + shift.x, 1 + shift.y, t + shift.z, 1.0f, nullptr, "v5"));
  vertices.push_back(new core::Vertex(0 + shift.x, -1 + shift.y, -t + shift.z, 1.0f, nullptr, "v6"));
  vertices.push_back(new core::Vertex(0 + shift.x, 1 + shift.y, -t + shift.z, 1.0f, nullptr, "v7"));

  vertices.push_back(new core::Vertex(t + shift.x, 0 + shift.y, -1 + shift.z, 1.0f, nullptr, "v8"));
  vertices.push_back(new core::Vertex(t + shift.x, 0 + shift.y, 1 + shift.z, 1.0f, nullptr, "v9"));
  vertices.push_back(new core::Vertex(-t + shift.x, 0 + shift.y, -1 + shift.z, 1.0f, nullptr, "v10"));
  vertices.push_back(new core::Vertex(-t + shift.x, 0 + shift.y, 1 + shift.z, 1.0f, nullptr, "v11"));

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
 * @param shift Deslocamento do polígono
 * @return models::Mesh* Ponteiro para a malha do polígono
 */
models::Mesh *shapes::polygon(int sides, float radius, core::Vector3 shift)
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
    core::Vertex *vertex = new core::Vertex(x + shift.x, y + shift.y, 0.0f + shift.z, 1.0f, nullptr, "v" + std::to_string(i));
    vertices.push_back(vertex);
  }

  // Define as faces (triângulos) que conectam os vértices ao centro
  core::Vertex *center = new core::Vertex(0.0f + shift.x, 0.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "center");
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
 * @param shift Deslocamento do cilindro
 * @return models::Mesh* Ponteiro para a malha do cilindro
 *
 */
models::Mesh *shapes::cone(float radius, float height, int segments, core::Vector3 shift)
{
  std::vector<core::Vertex *> vertexes;
  std::vector<std::vector<int>> faces;

  for (int i = 0; i < segments; i++)
  {
    float theta = static_cast<float>(i) / segments;
    float ratio = 2.0f * PI * theta;
    float x = radius * std::cosf(ratio);
    float y = radius * std::sinf(ratio);

    core::Vertex *vertex = new core::Vertex(x + shift.x, y, 0.0f + shift.y, 1.0f + shift.z, nullptr, "v" + std::to_string(i));
    vertexes.push_back(vertex);
  }

  // Adiciona o vértice central da base
  core::Vertex *center = new core::Vertex(0.0f + shift.x, 0.0f + shift.y, 0.0f + shift.z, 1.0f, nullptr, "center");
  vertexes.push_back(center);
  int centerIndex = vertexes.size() - 1;

  // Cria a face da base (sentido anti-horário)
  for (int i = 0; i < segments; i++)
  {
    int next = (i + 1) % segments;
    faces.push_back({i, centerIndex, next});
  }

  // Adiciona o vértice central do topo
  core::Vertex *top = new core::Vertex(0.0f + shift.x, 0.0f + shift.y, height + shift.z, 1.0f, nullptr, "top");
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

/**
 * @brief Cria um torus (rosquinha)
 *
 * @param majorRadius Raio principal (do centro ao tubo)
 * @param minorRadius Raio do tubo
 * @param majorSegments Segmentos ao redor do centro
 * @param minorSegments Segmentos ao redor do tubo
 * @param shift Deslocamento do torus
 * @return models::Mesh* Malha do torus
 * @todo Entender o que está quebrando o torus no recorte 3d
 * @note O torus não funciona corretamente com o recorte 3d pois o recorte deve ser feito em coordenadas homogêneas
 */
models::Mesh *shapes::torus(float majorRadius, float minorRadius, int majorSegments, int minorSegments, core::Vector3 shift)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  for (int i = 0; i < majorSegments; i++)
  {
    float majorAngle = 2.0f * PI * i / majorSegments;
    float cosMajor = cos(majorAngle);
    float sinMajor = sin(majorAngle);

    for (int j = 0; j < minorSegments; j++)
    {
      float minorAngle = 2.0f * PI * j / minorSegments;
      float cosMinor = cos(minorAngle);
      float sinMinor = sin(minorAngle);

      // Posição do vértice
      float x = (majorRadius + minorRadius * cosMinor) * cosMajor + shift.x;
      float y = (majorRadius + minorRadius * cosMinor) * sinMajor + shift.y;
      float z = minorRadius * sinMinor + shift.z;

      core::Vertex *v = new core::Vertex(x, y, z, 1.0f, nullptr, "v_" + std::to_string(i) + "_" + std::to_string(j));
      vertices.push_back(v);
    }
  }

  // Faces (quadrados divididos em triângulos)
  for (int i = 0; i < majorSegments; i++)
  {
    for (int j = 0; j < minorSegments; j++)
    {
      int nextI = (i + 1) % majorSegments;
      int nextJ = (j + 1) % minorSegments;

      int v0 = i * minorSegments + j;
      int v1 = i * minorSegments + nextJ;
      int v2 = nextI * minorSegments + nextJ;
      int v3 = nextI * minorSegments + j;

      faces.push_back({v0, v1, v2});
      faces.push_back({v0, v2, v3});
    }
  }

  return new models::Mesh(vertices, faces, "torus");
}

/**
 * @brief Cria um cilindro com topo e base circulares
 *
 * @param radius Raio da base/topo
 * @param height Altura do cilindro
 * @param segments Número de segmentos (resolução)
 * @param shift Deslocamento do cilindro
 * @return models::Mesh* Malha do cilindro
 */
models::Mesh *shapes::cylinder(float radius, float height, int segments, core::Vector3 shift)
{
  std::vector<core::Vertex *> vertices;
  std::vector<std::vector<int>> faces;

  // Vértices da base
  for (int i = 0; i < segments; i++)
  {
    float theta = 2.0f * PI * i / segments;
    float x = radius * cos(theta) + shift.x;
    float z = radius * sin(theta) + shift.z;
    core::Vertex *v = new core::Vertex(x, shift.y, z, 1.0f, nullptr, "v_base_" + std::to_string(i));
    vertices.push_back(v);
  }

  // Vértices do topo
  for (int i = 0; i < segments; i++)
  {
    float theta = 2.0f * PI * i / segments;
    float x = radius * cos(theta) + shift.x;
    float z = radius * sin(theta) + shift.z;
    core::Vertex *v = new core::Vertex(x, shift.y + height, z, 1.0f, nullptr, "v_top_" + std::to_string(i));
    vertices.push_back(v);
  }

  // Centro da base e topo
  core::Vertex *baseCenter = new core::Vertex(shift.x, shift.y, shift.z, 1.0f, nullptr, "base_center");
  vertices.push_back(baseCenter);
  int baseCenterIdx = vertices.size() - 1;

  core::Vertex *topCenter = new core::Vertex(shift.x, shift.y + height, shift.z, 1.0f, nullptr, "top_center");
  vertices.push_back(topCenter);
  int topCenterIdx = vertices.size() - 1;

  // Faces da base e topo (triângulos)
  for (int i = 0; i < segments; i++)
  {
    int next = (i + 1) % segments;
    // Base
    faces.push_back({i, next, baseCenterIdx});
    // Topo
    faces.push_back({segments + i, topCenterIdx, segments + next});
  }

  // Faces laterais (quadrados divididos em triângulos)
  for (int i = 0; i < segments; i++)
  {
    int next = (i + 1) % segments;
    int currentBase = i;
    int currentTop = segments + i;
    int nextBase = next;
    int nextTop = segments + next;

    faces.push_back({currentBase, currentTop, nextTop});
    faces.push_back({currentBase, nextTop, nextBase});
  }

  return new models::Mesh(vertices, faces, "cylinder");
}