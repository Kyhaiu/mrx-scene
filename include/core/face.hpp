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

#include <vector>
#include <string>
#include <limits> // Include this for std::numeric_limits

#include <core/vertex.hpp>
#include <core/halfedge.hpp>

#include <math/math.hpp>

#include <utils/nlohmann/json.hpp>

using json = nlohmann::json;

namespace core
{

  typedef struct Plane
  {
    core::Vector3 normal;                // Vetor normal ao plano
    float d;                             // Distância do plano à origem
    std::vector<core::Vector3> vertices; // Vértices que definem o plano

    // Construtor com normal e d
    Plane(core::Vector3 normal, float d) : normal(normal), d(d) {}

    // Construtor a partir de uma lista de vértices
    Plane(const std::vector<core::Vector3> &vertices) : vertices(vertices)
    {
      if (vertices.size() < 3)
      {
        throw std::invalid_argument("Um plano precisa de pelo menos 3 pontos.");
      }

      // Calcular a normal usando os três primeiros vértices
      core::Vector3 p1 = vertices[0];
      core::Vector3 p2 = vertices[1];
      core::Vector3 p3 = vertices[2];

      this->normal = math::Vector3Normalize(math::Vector3CrossProduct((p2 - p1), (p3 - p1)));
      this->d = -math::Vector3DotProduct(this->normal, p1);
    }

    // Calcula o ponto de interseção entre uma linha e o plano
    core::Vector3 intersectionPoint(core::Vector3 p1, core::Vector3 p2)
    {
      return p1 + (p2 - p1) * (-distance(p1) / math::Vector3DotProduct(normal, p2 - p1));
    }

    // Inverter o sentido da normal
    void invertNormal()
    {
      normal = math::Vector3Negate(normal);
      d = -d;
    }

    // Calcula a distância de um ponto ao plano
    float distance(core::Vector3 p) const
    {
      return math::Vector3DotProduct(normal, p) - d;
    }

    // Verifica se todos os vértices estão no mesmo plano
    bool isCoplanar() const
    {
      for (const auto &vertex : vertices)
      {
        if (std::abs(distance(vertex)) > EPSILON) // Tolerância para flutuações numéricas
        {
          return false;
        }
      }
      return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const Plane &plane)
    {
      os << "Normal: " << plane.normal << std::endl;
      os << "D: " << plane.d << std::endl;
      os << "Vertices: " << std::endl;
      for (const auto &vertex : plane.vertices)
      {
        os << vertex << std::endl;
      }
      return os;
    }
  } Plane;

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

    /**
     * @brief Vetor normal da face
     */
    core::Vector3 normal;

  public:
    /**
     * @brief Vetor que armazena os vértices da face após o clipping
     *
     * @note O clipping é feito em coordenadas de tela
     * @note A ordem no sentido anti-horário é preservada neste vetor
     */
    std::vector<core::Vector3> clipped_vertex;

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

    bool getVisible() const;
    void setVisible(bool is_visible);

    bool getHole() const;
    void setHole(bool is_hole);

    std::string getId() const;
    void setId(const std::string &id);

    core::Vector3 getNormal() const;

    // Methods
    bool isHole();
    bool isConterClockwise(core::Vector3 a, core::Vector3 b);
    bool isVisible(const core::Vector3 camera_position);
    bool isVisibleAltered(const core::Vector3 n);
    void getFaceNormal();
    core::Vector3 getFaceCentroid(bool screen_coords = false);

    core::Plane face2plane();

    // Friend functions
    friend std::ostream &operator<<(std::ostream &os, const Face &face);
  };
} // namespace core