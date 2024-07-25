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

    std::vector<core::Vertex *> vertexes;
    std::vector<core::Face *> faces;
    std::vector<core::HalfEdge *> mesh;

    // create vertexes of the mesh
    for (auto &v : this->getVertices())
    {
      vertexes.push_back(v);
    }

    // create faces of the mesh
    for (int i = 0; i < index_faces.size(); i++)
    {
      faces.push_back(new core::Face());
      faces[i]->setId("f" + std::to_string(i));
    }

    // variable to store the index of the half edge (just for the id)
    int k = 0;
    // create half edges of the mesh
    for (int i = 0; i < index_faces.size(); i++)
    {
      // get the number of vertices of the face
      int num_vertices = index_faces[i].size();

      // vector to store the half edges of the face
      std::vector<core::HalfEdge *> face_half_edges = {};

      // create the half edges of the face
      for (int j = 0; j < num_vertices; j++)
      {
        core::HalfEdge *he = new core::HalfEdge();
        he->setId("he" + std::to_string(k));
        mesh.push_back(he);
        face_half_edges.push_back(he);
        k++;
      }

      // set the half edges of the face
      for (int j = 0; j < num_vertices; j++)
      {
        core::HalfEdge *he = face_half_edges[j];
        he->setOrigin(vertexes[index_faces[i][j]]);

        if (he->getOrigin()->getHalfEdge() == nullptr)
        {
          // associate the vertex with the half edge
          he->getOrigin()->setHalfEdge(he);
        }

        he->setNext(face_half_edges[(j + 1) % num_vertices]);
        he->setPrev(face_half_edges[(j - 1 + num_vertices) % num_vertices]);
        he->setFace(faces[i]);
      }

      faces[i]->setHalfEdge(face_half_edges[0]);
      // faces[i]->setEdges(face_half_edges);
      // print the face_half_edges

      std::vector<core::Vertex *> vertices_face = {};

      for (int l = 0; l < face_half_edges.size(); l++)
      {
        vertices_face.push_back(face_half_edges[l]->getOrigin());
      }

      faces[i]->setVertex(vertices_face);
    }

    // set the twin of the half edges
    for (int i = 0; i < mesh.size(); i++)
    {
      core::HalfEdge *he = mesh[i];

      if (he->getTwin() == nullptr)
      {
        for (int j = i + 1; j < mesh.size(); j++)
        {
          core::HalfEdge *twin = mesh[j];

          if (he->getOrigin() == twin->getNext()->getOrigin() && he->getNext()->getOrigin() == twin->getOrigin())
          {
            he->setTwin(twin);
            twin->setTwin(he);
            break;
          }
        }
      }
    }

    // set the half edges of the vertexes
    this->setVertices(vertexes);
    this->setHalfEdges(mesh);
    this->setFaces(faces);
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
} // namespace models