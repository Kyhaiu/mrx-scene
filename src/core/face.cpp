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
    this->setVisible(face.isVisible());
    this->setHole(face.isHole());
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
  bool Face::isVisible() const
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
  bool Face::isHole() const
  {
    return this->is_hole;
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
    this->setVisible(face.isVisible());
    this->setHole(face.isHole());
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
   * @brief Método para verificar se a face é visível
   *
   * TODO: Implementar método para verificar se a face é visível
   * @return bool
   */
  bool Face::isVisible()
  {
    return false;
  }
} // namespace core