/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxhalfedge v1.0 - Estrutura de dados e funções básicas para trabalhar com arestas
 *
 *   CONVENTIONS: (Convenções)
 *    - As arestas são sempre orientadas no sentido anti-horário
 *    - As arestas são sempre duplas, ou seja, possuem sempre uma aresta gêmea
 *    - As arestas são sempre associadas a uma face
 *    - As arestas são sempre associadas a um vértice de origem
 *    - As arestas são sempre associadas a uma aresta anterior e a uma próxima aresta
 *
 *   IDIOM: ENGLISH
 *
 *   mrxhalfedge v1.0 - Data structure and basic functions to work with edges
 *
 *   CONVENTIONS:
 *    - The edges are always oriented counterclockwise
 *    - The edges are always double, that is, they always have a twin edge
 *    - The edges are always associated with a face
 *    - The edges are always associated with an origin vertex
 *    - The edges are always associated with a previous edge and a next edge
 *
 *   CONFIGURATION:
 *
 *   DEPENDENCIES:
 *     <core/vertex.hpp> - Required for: type Vertex
 *     <core/face.hpp> - Required for: type Face
 *     <iostream> - Required for: std::ostream
 *     <string> - Required for: std::strings
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *
 *   LICENSE: GPL 3.0
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************/
#pragma once

#include <core/common.hpp>
#include <string>

namespace core
{

  class HalfEdge
  {
  private:
    // Attributes

    /**
     * @brief Ponteiro para a próxima meia aresta
     */
    core::HalfEdge *next;

    /**
     * @brief Ponteiro para a meia aresta anterior
     */
    core::HalfEdge *prev;

    /**
     * @brief Ponteiro para a meia aresta gêmea
     */
    core::HalfEdge *twin;

    /**
     * @brief Ponteiro para o vértice de origem da meia aresta
     */
    core::Vertex *origin;

    /**
     * @brief Ponteiro para a face que contém a meia aresta
     */
    core::Face *face;

    /**
     * @brief Identificador da meia aresta
     */
    std::string id;

  public:
    // Constructors and destructors
    HalfEdge();
    ~HalfEdge();

    // Copy constructor
    HalfEdge(const HalfEdge &halfEdge);

    // Getters and setters
    core::HalfEdge *getNext() const;
    void setNext(core::HalfEdge *next);

    core::HalfEdge *getPrev() const;
    void setPrev(core::HalfEdge *prev);

    core::HalfEdge *getTwin() const;
    void setTwin(core::HalfEdge *twin);

    core::Vertex *getOrigin() const;
    void setOrigin(core::Vertex *origin);

    core::Face *getFace() const;
    void setFace(core::Face *face);

    std::string getId() const;
    void setId(const std::string &id);

    friend std::ostream &operator<<(std::ostream &os, const HalfEdge &he);
  };
} // namespace core