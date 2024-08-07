#include <core/vertex.hpp>

#include <iostream>

namespace core
{

  // ------------------------------------------------------------------------------------------
  // Constructors and Destructors
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Construtor da classe Vertex
   *
   * Inicializa o vetor da classe com o vetor 4D nulo e o id com uma string vazia.
   *
   */
  Vertex::Vertex()
  {
    this->setVector(core::Vector4Zero());
    this->setId("");
  }

  /**
   * @brief Construtor da classe Vertex
   *
   * @param x Coordenada x do vetor
   * @param y Coordenada y do vetor
   * @param z Coordenada z do vetor
   * @param w Coordenada w do vetor
   * @param half_edge Ponteiro para a meia aresta que tem o vértice como origem
   * @param id Identificador do vértice
   */
  Vertex::Vertex(float x, float y, float z, float w, core::HalfEdge *half_edge, std::string id)
  {
    this->setVector({x, y, z, w});
    // 1.17549e-38 is the smallest positive float value
    this->setVectorScreen({1.17549e-38f, 1.17549e-38f, 1.17549e-38f});
    this->setId(id);
    this->setHalfEdge(half_edge);
  }

  /**
   * @brief Destrutor da classe Vertex
   *
   */
  Vertex::~Vertex()
  {
  }

  /**
   * @brief Construtor de cópia da classe Vertex
   *
   * @param vertex
   */
  Vertex::Vertex(const Vertex &vertex)
  {
    this->setVector(vertex.getVector());
    this->setId(vertex.getId());
  }

  /**
   * @brief Operador de atribuição da classe Vertex
   *
   * @param vertex
   * @return Vertex&
   */
  Vertex &Vertex::operator=(const Vertex &vertex)
  {
    this->setVector(vertex.getVector());
    this->setId(vertex.getId());
    return *this;
  }

  // ------------------------------------------------------------------------------------------
  // Methods - Getters and Setters
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Método get do vetor da classe Vertex
   *
   * Retorna uma cópia do vetor 4D da classe Vertex.
   *
   * @return MRX_VECTOR4_TYPE
   */
  MRX_VECTOR4_TYPE Vertex::getVector() const
  {
    return this->vector;
  }

  /**
   * @brief Método set do vetor da classe Vertex
   *
   * @param vector Vetor 4D a ser atribuído à classe Vertex
   */
  void Vertex::setVector(const MRX_VECTOR4_TYPE &vector)
  {
    this->vector = vector;
  }

  /**
   * @brief Método get do vetor de tela da classe Vertex
   *
   * Retorna uma cópia do vetor 3D com valores das componentes
   * x, y e z convertidos para coordenadas de tela.
   *
   * @return MRX_VECTOR3_TYPE
   */
  MRX_VECTOR3_TYPE Vertex::getVectorScreen()
  {
    return this->_vector_screen;
  }

  /**
   * @brief Método set do vetor de tela da classe Vertex
   *
   * @param vector Vetor 3D a ser atribuído à classe Vertex
   */
  void Vertex::setVectorScreen(const MRX_VECTOR3_TYPE &vector)
  {
    this->_vector_screen = vector;
  }

  /**
   * @brief Método get do identificador da classe Vertex
   *
   * Retorna o id do vetor.
   *
   * @return std::string
   */
  std::string Vertex::getId() const
  {
    return this->id;
  }

  /**
   * @brief Método set do vetor de tela da classe Vertex
   *
   * @param std::string Identificador a ser atribuído à classe Vertex
   */
  void Vertex::setId(const std::string &id)
  {
    this->id = id;
  }

  /**
   * @brief Método get da coordenada x da classe Vertex
   *
   * Retorna a coordenada x do vetor 4D da classe Vertex.
   * ou a coordenada x do vetor de tela, caso o parâmetro screen_coord seja verdadeiro.
   *
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   * @return float Coordenada x do vetor
   */
  float Vertex::getX(bool screen_coord) const
  {
    return screen_coord ? this->_vector_screen.x : this->vector.x;
  }

  /**
   * @brief Método set da coordenada x da classe Vertex
   *
   * @param x Coordenada x a ser atribuída ao vetor
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   */
  void Vertex::setX(float x, bool screen_coord)
  {
    if (screen_coord)
    {
      this->_vector_screen.x = x;
    }
    else
    {
      this->vector.x = x;
    }
  }

  /**
   * @brief Método get da coordenada y da classe Vertex
   *
   * Retorna a coordenada y do vetor 4D da classe Vertex.
   * ou a coordenada y do vetor de tela, caso o parâmetro screen_coord seja verdadeiro.
   *
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   * @return float Coordenada y do vetor
   */
  float Vertex::getY(bool screen_coord) const
  {
    return screen_coord ? this->_vector_screen.y : this->vector.y;
  }

  /**
   * @brief Método set da coordenada y da classe Vertex
   *
   * @param y Coordenada y a ser atribuída ao vetor
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   */
  void Vertex::setY(float y, bool screen_coord)
  {
    if (screen_coord)
    {
      this->_vector_screen.y = y;
    }
    else
    {
      this->vector.y = y;
    }
  }

  /**
   * @brief Método get da coordenada z da classe Vertex
   *
   * Retorna a coordenada z do vetor 4D da classe Vertex.
   * ou a coordenada z do vetor de tela, caso o parâmetro screen_coord seja verdadeiro.
   *
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   * @return float Coordenada z do vetor
   */
  float Vertex::getZ(bool screen_coord) const
  {
    return screen_coord ? this->_vector_screen.z : this->vector.z;
  }

  /**
   * @brief Método set da coordenada z da classe Vertex
   *
   * @param z Coordenada z a ser atribuída ao vetor
   * @param screen_coord Flag para indicar se a coordenada é do vetor de tela
   * @note O valor padrão de `screen_coord` é falso
   */
  void Vertex::setZ(float z, bool screen_coord)
  {
    if (screen_coord)
    {
      this->_vector_screen.z = z;
    }
    else
    {
      this->vector.z = z;
    }
  }

  /**
   * @brief Método get da coordenada w da classe Vertex
   *
   * Retorna a coordenada w do vetor 4D da classe Vertex.
   *
   * @return float Coordenada w do vetor
   */
  float Vertex::getW(bool screen_coord) const
  {
    return this->vector.w;
  }

  /**
   * @brief Método set da coordenada w da classe Vertex
   *
   * @param w Coordenada w a ser atribuída ao vetor
   */
  void Vertex::setW(float w, bool screen_coord)
  {
    this->vector.w = w;
  }

  /**
   * @brief Método get da meia aresta da classe Vertex
   *
   * Retorna a meia aresta que tem o vértice como origem.
   *
   * @return core::HalfEdge*
   */
  core::HalfEdge *Vertex::getHalfEdge() const
  {
    return this->half_edge;
  }

  /**
   * @brief Método set da meia aresta da classe Vertex
   *
   * @param half_edge Ponteiro para a meia aresta a ser atribuída ao vértice
   */
  void Vertex::setHalfEdge(core::HalfEdge *half_edge)
  {
    this->half_edge = half_edge;
  }

  // ------------------------------------------------------------------------------------------
  // Methods
  // ------------------------------------------------------------------------------------------s

  /**
   * @brief Método para normalizar o vetor da classe Vertex
   *
   * @return MRX_VECTOR4_TYPE
   */
  MRX_VECTOR4_TYPE Vertex::normalize() const
  {
    core::Vector3 vec = math::Vector3Normalize({this->vector.x, this->vector.y, this->vector.z});
    return {vec.x, vec.y, vec.z, this->vector.w};
  }

  /**
   * @brief Método para converter o vetor de 4D para uma matriz de 4x1
   *
   * @note Utilizado para aplicar transformações de matriz ao vetor
   * @return core::float4
   */
  core::float4 Vertex::toArray() const
  {
    return {this->vector.x, this->vector.y, this->vector.z, this->vector.w};
  }

  std::ostream &operator<<(std::ostream &os, const core::Vertex &v)
  {
    os << v.id << ": (" << v.vector.x << ", \t" << v.vector.y << ", \t" << v.vector.z << ", \t" << v.vector.w << ")" << "\t" << v._vector_screen.x << ", \t" << v._vector_screen.y << ", \t" << v._vector_screen.z << std::endl;
    return os;
  }

} // namespace core