#include <core/halfedge.hpp>
#include <core/vertex.hpp>
#include <core/face.hpp>
#include <iostream>

namespace core
{
  /**
   * @brief Construtor padrão da classe HalfEdge
   */
  HalfEdge::HalfEdge()
  {
    this->next = nullptr;
    this->prev = nullptr;
    this->twin = nullptr;
    this->origin = nullptr;
    this->face = nullptr;
    this->id = "";
  }

  /**
   * @brief Construtor de cópia da classe HalfEdge
   */
  HalfEdge::HalfEdge(const HalfEdge &halfEdge)
  {
    this->next = halfEdge.next;
    this->prev = halfEdge.prev;
    this->twin = halfEdge.twin;
    this->origin = halfEdge.origin;
    this->face = halfEdge.face;
    this->id = halfEdge.id;
  }

  /**
   * @brief Destrutor da classe HalfEdge
   */
  HalfEdge::~HalfEdge()
  {
    this->next = nullptr;
    this->prev = nullptr;
    this->twin = nullptr;
    this->origin = nullptr;
    this->face = nullptr;
  }

  /**
   * @brief Método para obter o ponteiro para a próxima meia aresta
   * @return Ponteiro para a próxima meia aresta
   */
  HalfEdge *HalfEdge::getNext() const
  {
    return this->next;
  }

  /**
   * @brief Método para definir o ponteiro para a próxima meia aresta
   * @param next Ponteiro para a próxima meia aresta
   */
  void HalfEdge::setNext(HalfEdge *next)
  {
    this->next = next;
  }

  /**
   * @brief Método para obter o ponteiro para a meia aresta anterior
   * @return Ponteiro para a meia aresta anterior
   */
  HalfEdge *HalfEdge::getPrev() const
  {
    return this->prev;
  }

  /**
   * @brief Método para definir o ponteiro para a meia aresta anterior
   * @param prev Ponteiro para a meia aresta anterior
   */
  void HalfEdge::setPrev(HalfEdge *prev)
  {
    this->prev = prev;
  }

  /**
   * @brief Método para obter o ponteiro para a meia aresta gêmea
   * @return Ponteiro para a meia aresta gêmea
   */
  HalfEdge *HalfEdge::getTwin() const
  {
    return this->twin;
  }

  /**
   * @brief Método para definir o ponteiro para a meia aresta gêmea
   * @param twin Ponteiro para a meia aresta gêmea
   */
  void HalfEdge::setTwin(HalfEdge *twin)
  {
    this->twin = twin;
  }

  /**
   * @brief Método para obter o ponteiro para o vértice de origem da meia aresta
   * @return Ponteiro para o vértice de origem da meia aresta
   */
  Vertex *HalfEdge::getOrigin() const
  {
    return this->origin;
  }

  /**
   * @brief Método para definir o ponteiro para o vértice de origem da meia aresta
   * @param origin Ponteiro para o vértice de origem da meia aresta
   */
  void HalfEdge::setOrigin(Vertex *origin)
  {
    this->origin = origin;
  }

  /**
   * @brief Método para obter o ponteiro para a face que contém a meia aresta
   * @return Ponteiro para a face que contém a meia aresta
   */
  Face *HalfEdge::getFace() const
  {
    return this->face;
  }

  /**
   * @brief Método para definir o ponteiro para a face que contém a meia aresta
   * @param face Ponteiro para a face que contém a meia aresta
   */
  void HalfEdge::setFace(Face *face)
  {
    this->face = face;
  }

  /**
   * @brief Método para obter o identificador da meia aresta
   * @return Identificador da meia aresta
   */
  std::string HalfEdge::getId() const
  {
    return this->id;
  }

  /**
   * @brief Método para definir o identificador da meia aresta
   * @param id Identificador da meia aresta
   */
  void HalfEdge::setId(const std::string &id)
  {
    this->id = id;
  }

  /**
   * @brief Sobrecarga do operador de inserção em stream
   *
   * @param os Stream de saída
   * @param he Meia aresta a ser inserida na stream
   * @return std::ostream& Stream de saída
   */
  std::ostream &operator<<(std::ostream &os, const HalfEdge &he)
  {

    os << he.id << ": (\n";
    os << "\torigin: " << he.getOrigin()->getId() << "\n";
    os << "\tprev: " << he.prev->getOrigin()->getId() << "\n";
    os << "\tnext: " << he.next->getOrigin()->getId() << "\n";
    os << "\ttwin: " << he.twin->getOrigin()->getId() << "\n";
    os << "\tface: " << he.face->getId() << "\n"
       << ")";
    return os;
  }
} // namespace core