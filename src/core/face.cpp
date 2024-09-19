#include <core/face.hpp>
#include <core/halfedge.hpp>
#include <core/vertex.hpp>

#include <iostream>

namespace core
{
  // ------------------------------------------------------------------------------------------
  // Constructors and Destructors
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Construtor da classe Face
   *
   * Inicializa os ponteiros da classe com nullptr, as flags com false e o id com uma string vazia.
   *
   */
  Face::Face()
  {
    this->setVertex({});
    this->setHalfEdge(nullptr);
    this->setVisible(false);
    this->setHole(false);
    this->setId("");
  }

  /**
   * @brief Destrutor da classe Face
   *
   */
  Face::~Face()
  {
  }

  /**
   * @brief Construtor de cópia da classe Face
   *
   * @param face
   */
  Face::Face(const Face &face)
  {
    this->setVertex(face.getVertex());
    this->setHalfEdge(face.getHalfEdge());
    this->setVisible(face.getVisible());
    this->setHole(face.getHole());
    this->setId(face.getId());
  }

  // ------------------------------------------------------------------------------------------
  // Methods - Getters and Setters
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Método get do vetor de ponteiros para os vértices da face
   *
   * @return std::vector<core::Vertex*>
   */
  std::vector<core::Vertex *> Face::getVertex() const
  {
    return this->vertex;
  }

  /**
   * @brief Método get do ponteiro para o vértice da face
   *
   * @param index
   * @return core::Vertex*
   */
  core::Vertex *Face::getVertex(int index) const
  {
    return this->vertex[index];
  }

  /**
   * @brief Método set do vetor de ponteiros para os vértices da face
   *
   * @param vertexes
   */
  void Face::setVertex(std::vector<core::Vertex *> vertexes)
  {
    this->vertex = vertexes;
  }

  /**
   * @brief Método set do ponteiro para o vértice da face
   *
   * @param index
   * @param vertex
   */
  void Face::setVertex(core::Vertex *vertex, int index)
  {
    this->vertex[index] = vertex;
  }

  /**
   * @brief Método get do ponteiro para a meia aresta da face
   *
   * @return core::HalfEdge*
   */
  core::HalfEdge *Face::getHalfEdge() const
  {
    return this->halfEdge;
  }

  /**
   * @brief Método set do ponteiro para a meia aresta da face
   *
   * @param halfEdge
   */
  void Face::setHalfEdge(core::HalfEdge *halfEdge)
  {
    this->halfEdge = halfEdge;
  }

  /**
   * @brief Método get da flag de visibilidade da face
   *
   * @return bool
   */
  bool Face::getVisible() const
  {
    return this->is_visible;
  }

  /**
   * @brief Método set da flag de visibilidade da face
   *
   * @param is_visible
   */
  void Face::setVisible(bool is_visible)
  {
    this->is_visible = is_visible;
  }

  /**
   * @brief Método get da flag de buraco da face
   *
   * @return bool
   */
  bool Face::getHole() const
  {
    return this->is_hole;
  }

  /**
   * @brief Método get do vetor normal da face
   *
   */
  core::Vector3 Face::getNormal() const
  {
    return this->normal;
  }

  /**
   * @brief Método set da flag de buraco da face
   *
   * @param is_hole
   */
  void Face::setHole(bool is_hole)
  {
    this->is_hole = is_hole;
  }

  /**
   * @brief Método get do id da face
   *
   * @return std::string
   */
  std::string Face::getId() const
  {
    return this->id;
  }

  /**
   * @brief Método set do id da face
   *
   * @param id
   */
  void Face::setId(const std::string &id)
  {
    this->id = id;
  }

  // ------------------------------------------------------------------------------------------
  // Operators
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Operador de atribuição da classe Face
   *
   * @param face
   * @return Face&
   */
  Face &Face::operator=(const Face &face)
  {
    this->setVertex(face.getVertex());
    this->setHalfEdge(face.getHalfEdge());
    this->setVisible(face.getVisible());
    this->setHole(face.getHole());
    this->setId(face.getId());
    return *this;
  }

  std::ostream &operator<<(std::ostream &os, const Face &f)
  {
    const char *visible = f.is_visible ? "true" : "false";

    os << f.id << ": (\n"
       << "\tvertices: (\n";
    for (auto &v : f.getVertex())
    {
      os << "\t\t" << *v << ",\n";
    }
    os << "\t)\n"
       << "\thalfedge: " << f.getHalfEdge()->getId() << "\n"
       << "\tvisible: " << visible
       << "\n"
       << ")";
    return os;
  }

  // ------------------------------------------------------------------------------------------
  // Methods
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Função utilizada para saber o sentido dos vértices da face
   *
   * @param a  Vetor 1
   * @param b Vetor 2
   *
   * @return bool - Retorna true se o sentido dos vértices for anti-horário e false se for horário
   * @note O produto vetorial dos vetores A e B fornece a área do paralelogramo. Logo, metade disso é a área do triângulo.
   *
   * @note Se o valor da área for positivo, a face é orientada no sentido anti-horário. Caso contrário, é orientada no sentido horário.
   */
  bool Face::isConterClockwise(core::Vector3 a, core::Vector3 b)
  {
    return (0.5 * math::MatrixDeterminant({1, 1, 1, a.x, a.y, 0, b.x, b.y, 0})) > 0;
  }

  /**
   * @brief Verifica se a face é visível
   *
   * @param camera_position Posição da câmera
   *
   * @return bool
   *
   * @note A face é visível se o produto escalar entre o vetor normal da face e o vetor da face para a câmera for maior que 0
   */
  bool Face::isVisible(const core::Vector3 camera_position)
  {
    this->getFaceNormal();

    core::Vector3 centroid = this->getFaceCentroid();

    core::Vector3 face2camera = math::Vector3Normalize({camera_position.x - centroid.x,
                                                        camera_position.y - centroid.y,
                                                        camera_position.z - centroid.z});

    return math::Vector3DotProduct(this->normal, face2camera) > 0;
  }

  /**
   * @brief Calcula o vetor normal da face
   *
   * @return void - O vetor normal é armazenado no atributo normal da classe
   */
  void Face::getFaceNormal()
  {
    core::Vector3 p1, p2, p3;

    core::HalfEdge *he = this->getHalfEdge();

    p1 = he->getPrev()->getOrigin()->getVector().toVector3();
    p2 = he->getOrigin()->getVector().toVector3();
    p3 = he->getNext()->getOrigin()->getVector().toVector3();

    core::Vector3 a = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
    core::Vector3 b = {p3.x - p2.x, p3.y - p2.y, p3.z - p2.z};

    // Vetor normal da face = B x A
    this->normal = math::Vector3Normalize(math::Vector3CrossProduct(a, b));

    // Se a face não estiver orientada no sentido anti-horário, inverte o vetor normal
    if (!this->isConterClockwise(a, b))
    {
      this->normal.x *= -1;
      this->normal.y *= -1;
      this->normal.z *= -1;
    }
  }

  /**
   * @brief Obtém uma aproximação do centroide da face
   *
   * @param bool screen_coords Flag para indicar se o cálculo deve ser feito em coordenadas de tela
   *
   * @return core::Vector3 Vetor com a posição do centroide da face
   */
  core::Vector3 Face::getFaceCentroid(bool screen_coords)
  {

    core::Vector3 centroid = {0, 0, 0};

    for (auto vertex : this->getVertex())
    {
      centroid.x += vertex->getVector().x;
      centroid.y += vertex->getVector().y;
      centroid.z += vertex->getVector().z;
    }

    int size = this->getVertex().size();

    centroid.x /= size;
    centroid.y /= size;
    centroid.z /= size;

    return centroid;
  }
} // namespace core