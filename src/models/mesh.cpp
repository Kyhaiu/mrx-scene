#include <models/mesh.hpp>
#include <iostream>

namespace models
{
  //------------------------------------------------------------------------------------------------
  // Constructors and Destructors
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Construtor padrão da classe Mesh
   *
   * @note Inicializa o número de faces da malha como 0
   * @note Inicializa o identificador da malha como uma string vazia
   * @note Inicializa os vetores de vértices, faces e meias arestas como vetores vazios
   */
  Mesh::Mesh()
  {
    this->num_faces = 0;
    this->id = "";
    this->vertices = std::vector<core::Vertex *>();
    this->faces = std::vector<core::Face *>();
    this->half_edges = std::vector<core::HalfEdge *>();
  }

  /**
   * @brief Construtor da classe Mesh
   *
   * @param vertexes Vetor de ponteiros para objetos da classe Vertex
   * @param faces Vetor de vetores de inteiros que representam as faces da malha
   * @param id Identificador da malha
   */
  Mesh::Mesh(std::vector<core::Vertex *> vertexes, std::vector<std::vector<int>> faces, std::string id)
  {
    this->setVertices(vertexes);
    this->setNumFaces(faces.size());
    this->setId(id);
    this->createMesh(faces);
  }

  /**
   * @brief Destrutor padrão da classe Mesh
   */
  Mesh::~Mesh()
  {
  }

  /**
   * @brief Construtor de cópia da classe Mesh
   *
   * @param mesh Referência constante para um objeto da classe Mesh
   */
  Mesh::Mesh(const Mesh &mesh)
  {
    this->vertices = mesh.vertices;
    this->faces = mesh.faces;
    this->half_edges = mesh.half_edges;
    this->num_faces = mesh.num_faces;
    this->id = mesh.id;
  }

  /**
   * @brief Sobrecarga do operador de atribuição da classe Mesh
   *
   * @param mesh Referência constante para um objeto da classe Mesh
   */
  Mesh &Mesh::operator=(const Mesh &mesh)
  {
    this->vertices = mesh.vertices;
    this->faces = mesh.faces;
    this->half_edges = mesh.half_edges;
    this->num_faces = mesh.num_faces;
    this->id = mesh.id;
    return *this;
  }

  //------------------------------------------------------------------------------------------------
  // Getters and Setters
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Método que retorna o vetor de vértices da malha
   *
   * @return std::vector<core::Vertex> Vetor de vértices da malha
   */
  std::vector<core::Vertex *> Mesh::getVertices() const
  {
    return this->vertices;
  }

  /**
   * @brief Método que retorna o vetor de faces da malha
   *
   * @return std::vector<core::Face> Vetor de faces da malha
   */
  std::vector<core::Face *> Mesh::getFaces() const
  {
    return this->faces;
  }

  /**
   * @brief Método que retorna o vetor de meias arestas da malha
   *
   * @return std::vector<core::HalfEdge> Vetor de meias arestas da malha
   */
  std::vector<core::HalfEdge *> Mesh::getHalfEdges() const
  {
    return this->half_edges;
  }

  /**
   * @brief Método que retorna o número de faces da malha
   *
   * @return int Número de faces da malha
   */
  int Mesh::getNumFaces() const
  {
    return this->num_faces;
  }

  /**
   * @brief Método que retorna o identificador da malha
   *
   * @return std::string Identificador da malha
   */
  std::string Mesh::getId() const
  {
    return this->id;
  }

  /**
   * @brief Método que retorna se o objeto está selecionado
   *
   * @return bool Flag que indica se o objeto está selecionado
   */
  bool Mesh::isSelected() const
  {
    return this->selected;
  }

  /**
   * @brief Método que define o vetor de vértices da malha
   *
   * @param vertices Vetor de vértices da malha
   */
  void Mesh::setVertices(const std::vector<core::Vertex *> vertices)
  {
    this->vertices = vertices;
  }

  /**
   * @brief Método que define o vetor de faces da malha
   *
   * @param faces Vetor de faces da malha
   */
  void Mesh::setFaces(const std::vector<core::Face *> faces)
  {
    this->faces = faces;
  }

  /**
   * @brief Método que define o vetor de meias arestas da malha
   *
   * @param half_edges Vetor de meias arestas da malha
   */
  void Mesh::setHalfEdges(const std::vector<core::HalfEdge *> half_edges)
  {
    this->half_edges = half_edges;
  }

  /**
   * @brief Método que define o número de faces da malha
   *
   * @param num_faces Número de faces da malha
   */
  void Mesh::setNumFaces(int num_faces)
  {
    this->num_faces = num_faces;
  }

  /**
   * @brief Método que define o identificador da malha
   *
   * @param id Identificador da malha
   */
  void Mesh::setId(const std::string &id)
  {
    this->id = id;
  }

  /**
   * @brief Método que define se o objeto está selecionado
   *
   * @param selected Flag que indica se o objeto está selecionado
   */
  void Mesh::setSelected(bool selected)
  {
    this->selected = selected;
  }

  //------------------------------------------------------------------------------------------------
  // Functions
  //------------------------------------------------------------------------------------------------

  void Mesh::createMesh(std::vector<std::vector<int>> index_faces)
  {
    if (this->vertices.size() == 0)
    {
      throw std::runtime_error("No vertices to create mesh");
      return;
    }
    else if (this->vertices.size() < 3)
    {
      throw std::runtime_error("Not enough vertices to create mesh. At least 3 vertices are needed");
      return;
    }

    if (this->getNumFaces() == 0)
    {
      throw std::runtime_error("No faces to create mesh");
      return;
    }

    std::vector<core::Vertex *> vertices = this->getVertices();

    // Cria as faces da malha
    for (auto face : index_faces)
    {
      std::vector<core::Vertex *> face_vertices;
      for (auto index : face)
      {
        face_vertices.push_back(vertices[index]);
      }

      core::Face *f = this->addFaceByVertices(face_vertices);

      f->setVertex(face_vertices);
    }

    for (auto edge : this->half_edges)
    {
      if (edge->getTwin() == nullptr)
      {
        core::Vertex *v1 = edge->getOrigin();
        core::Vertex *v2 = edge->getNext()->getOrigin();

        core::HalfEdge *twin = this->findEdge(v2, v1);
        if (twin != nullptr)
        {
          edge->setTwin(twin);
          twin->setTwin(edge);
        }
      }
    }

    for (auto edge : this->half_edges)
    {
      if (edge->getFace() == nullptr)
      {
        core::HalfEdge *next = edge->getTwin();

        do
        {
          next = next->getPrev()->getTwin();
        } while (next->getFace() != nullptr);

        edge->setNext(next);
        next->setPrev(edge);
      }
    }

    // Inicialização dos outros atributos da malha
    this->setSelected(false);
    this->material.ambient = {0.5f, 0.0f, 0.0f};
    this->material.diffuse = {0.5f, 0.0f, 0.0f};
    this->material.specular = {0.9f, 0.9f, 0.9f};
    this->material.shininess = 3.0f;

    this->setNumFaces(this->faces.size());
  }

  /**
   * @brief Método que retorna o box envolvente da malha
   *
   * @param screen_coordinates Flag que indica se as coordenadas são em relação à tela
   *
   * @note x = min_x, y = min_y, z = max_x, w = max_y
   * @return core::Vector4 Box envolvente da malha
   */
  core::Vector4 Mesh::getBox(bool screen_coordinates)
  {
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::min();
    float max_y = std::numeric_limits<float>::min();

    float x, y;

    for (auto v : this->getVertices())
    {
      x = screen_coordinates ? v->getX(true) : v->getX();
      y = screen_coordinates ? v->getY(true) : v->getY();

      if (x < min_x)
      {
        min_x = x;
      }
      if (y < min_y)
      {
        min_y = y;
      }
      if (x > max_x)
      {
        max_x = x;
      }
      if (y > max_y)
      {
        max_y = y;
      }
    }

    core::Vector4 result = {min_x, min_y, max_x, max_y};

    return result;
  }

  /**
   * @brief Método que calcula as normais dos vértices da malha
   *
   */
  void Mesh::determineNormals()
  {

    for (auto v : this->getVertices())
    {
      core::HalfEdge *start_he = v->getHalfEdge();
      core::HalfEdge *he = start_he;

      core::Vector3 normal = {0.0f, 0.0f, 0.0f};

      // std::cout << "Determinando a normal do vertice: " << v->getId() << std::endl;
      // std::cout << "As faces que compõem o vertice são: " << std::endl;

      while (true)
      {

        core::Vector3 face_normal = he->getFace()->getNormal();

        // std::cout << he->getFace()->getId() << " ";
        // std::cout << "Normal da face: " << face_normal.x << " " << face_normal.y << " " << face_normal.z << std::endl;

        normal.x += face_normal.x;
        normal.y += face_normal.y;
        normal.z += face_normal.z;

        he = he->getTwin()->getNext();

        if (he == start_he)
          break;
      }

      float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

      // std::cout << "Normal do vertice: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
      // std::cout << "Comprimento da normal: " << length << std::endl;

      normal.x /= length;
      normal.y /= length;
      normal.z /= length;

      // std::cout << "Normal do vertice normalizada: " << normal.x << " " << normal.y << " " << normal.z << std::endl;

      v->setNormal(normal);
    }
  }

  core::HalfEdge *Mesh::addEdge(core::Vertex *vertex1, core::Vertex *vertex2)
  {
    core::HalfEdge *he = new core::HalfEdge();
    he->setId("e" + std::to_string(this->half_edges.size()));

    std::string id = "e" + vertex1->getId() + "-" + vertex2->getId();
    this->half_edges_map.insert(std::pair<std::string, core::HalfEdge *>(id, he));
    this->half_edges.push_back(he);

    he->setOrigin(vertex1);
    if (vertex1->getHalfEdge() == nullptr)
      vertex1->setHalfEdge(he);

    // Associação da aresta com o seu par, se ele existir
    core::HalfEdge *twin_he = this->findEdge(vertex2, vertex1);
    if (twin_he != nullptr)
    {
      he->setTwin(twin_he);
      twin_he->setTwin(he);
    }

    return he;
  }

  /**
   * @brief Método que adiciona uma face à malha a partir de um vetor de arestas
   *
   * @param half_edges Vetor de ponteiros para objetos da classe HalfEdge
   * @return core::Face* Ponteiro para a face criada
   */
  core::Face *Mesh::addFaceByHalfEdges(std::vector<core::HalfEdge *> half_edges)
  {
    core::Face *face = new core::Face();
    face->setId("f" + std::to_string(this->faces.size()));
    // Adiciona a face a malha
    this->faces.push_back(face);

    // Associa as meias arestas a face
    for (int i = 0; i < half_edges.size(); i++)
    {
      half_edges[i]->setFace(face);
    }

    // Associa a primeira meia aresta a face
    face->setHalfEdge(half_edges[0]);

    // Conecta as meias arestas ao redor da face
    int len = half_edges.size();
    for (int i = 0; i < half_edges.size(); i++)
    {
      half_edges[i]->setNext(half_edges[(i + 1) % len]);
      half_edges[i]->setPrev(half_edges[(i - 1 + len) % len]);
    }

    return face;
  }

  core::Face *Mesh::addFaceByVertices(std::vector<core::Vertex *> vertices)
  {
    std::vector<core::HalfEdge *> half_edges;
    int len = vertices.size();
    for (int i = 0; i < len; i++)
    {
      core::Vertex *v1 = vertices[i];
      core::Vertex *v2 = vertices[(i + 1) % len];

      core::HalfEdge *he = this->findEdge(v1, v2);
      // Se a aresta não existir, cria uma nova
      if (he == nullptr)
        he = this->addEdge(v1, v2);

      half_edges.push_back(he);
    }

    return this->addFaceByHalfEdges(half_edges);
  }

  /**
   * @brief Método que busca uma aresta no mapa de arestas da malha
   *
   * @param vertex1 Vértice 1
   * @param vertex2 Vértice 2
   * @return core::HalfEdge* Ponteiro para a aresta encontrada
   *
   * @note Se a aresta não for encontrada, retorna nullptr
   */
  core::HalfEdge *Mesh::findEdge(core::Vertex *vertex1, core::Vertex *vertex2)
  {
    std::string key = "e" + vertex1->getId() + "-" + vertex2->getId();
    if (this->half_edges_map.find(key) != this->half_edges_map.end())
      return this->half_edges_map[key];

    return nullptr;
  }

  /**
   * @brief Método que verifica se a malha está fora da viewport
   *
   * @param min_viewport Coordenada mínima da viewport (tela)
   * @param max_viewport Coordenada máxima da viewport (tela)
   * @return true Se a malha está fora da viewport
   * @return false Se a malha está dentro da viewport
   */
  bool Mesh::isOutsideViewport(core::Vector2 min_viewport, core::Vector2 max_viewport)
  {
    core::Vector4 box = this->getBox(true);

    if (box.x > max_viewport.x || box.z < min_viewport.x || box.y > max_viewport.y || box.w < min_viewport.y)
      return true;

    return false;
  }
} // namespace models