/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxface v1.0 - Estrutura de dados e funções básicas para trabalhar com faces
 *
 *   CONVENTIONS: (Convenções)
 *    - As faces são sempre orientadas no sentido anti-horário
 *    - O teste de visibilidade de uma face é feito através do produto escalar entre o vetor normal da face e o vetor direção da câmera
 *      - A matemática do teste é implementada fora da classe
 *
 *   IDIOM: ENGLISH
 *
 *   mrxface v1.0 - Data structure and basic functions to work with faces
 *
 *   CONVENTIONS:
 *    - The faces are always oriented counterclockwise
 *    - The visibility test of a face is done through the dot product between the face normal vector and the camera direction vector
 *      - The math of the test is implemented outside the class
 *
 *   CONFIGURATION:
 *
 *   DEPENDENCIES:
 *      <core/vertex.hpp> - Required for: type Vertex
 *      <core/halfedge.hpp> - Required for: type HalfEdge
 *      <iostream> - Required for: std::ostream
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

#include <core/vertex.hpp>
#include <core/halfedge.hpp>

#include <vector>
#include <string>

namespace core
{
  class Face
  {
  private:
    // Attributes

    /**
     * @brief Vetor para os vértices da face
     */
    std::vector<core::Vertex *> vertex;

    /**
     * @brief Ponteiro para uma meia aresta da face
     */
    core::HalfEdge *halfEdge;

    /**
     * @brief Flag para indicar se a face é visível
     */
    bool is_visible;

    /**
     * @brief Flag para indicar se a face é um buraco
     */
    bool is_hole;

    /**
     * @brief Identificador da face
     */
    std::string id;

  public:
    // Constructors and destructors
    Face();
    ~Face();

    // Copy constructor
    Face(const Face &face);

    // Copy assignment operator
    Face &operator=(const Face &face);

    // Getters and setters
    core::Vertex *getVertex(int index) const;
    std::vector<core::Vertex *> getVertex() const;
    void setVertex(std::vector<core::Vertex *> vertexes);
    void setVertex(core::Vertex *vertex, int index);

    core::HalfEdge *getHalfEdge() const;
    void setHalfEdge(core::HalfEdge *halfEdge);

    bool isVisible() const;
    void setVisible(bool is_visible);

    bool isHole() const;
    void setHole(bool is_hole);

    std::string getId() const;
    void setId(const std::string &id);

    // Methods
    bool isVisible();
    bool isHole();

    // Friend functions
    friend std::ostream &operator<<(std::ostream &os, const Face &face);
  };
} // namespace core