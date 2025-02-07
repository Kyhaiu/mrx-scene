/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxmesh v1.0 - Estrutura de dados e funções básicas para trabalhar com malhas poligonais
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *     - As funções sempre usam uma variável "result" para retorno
 *
 *   IDIOM: ENGLISH
 *
 *   mrxmesh v1.0 -
 *
 *   CONVENTIONS:

 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <cmath.h>    - Required for: sinf(), cosf(), tan(), atan2f(), sqrtf(), floor(), fminf(), fmaxf(), fabs()
 *      <core/vector.hpp> - Required for: types Vertex4
 *      <core/vertex.hpp> - Required for: class Vertex
 *      <core/face.hpp> - Required for: class Face
 *      <core/halfedge.hpp> - Required for: class HalfEdge
 *      <vector>    - Required for: std::vector
 *      <string>    - Required for: std::string
 *      <stdexcept> - Required for: standard exceptions
 *      <limits>    - Required for: std::numeric_limits
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
#include <stdexcept> // Include this for standard exceptions
#include <limits>    // Include this for std::numeric_limits
#include <map>       // Include this for std::map

#include <models/common.hpp>

#include <utils/nlohmann/json.hpp>

#include <core/vertex.hpp>
#include <core/face.hpp>
#include <core/halfedge.hpp>

using json = nlohmann::json;

namespace models
{

  class Mesh
  {
  private:
    /**
     * @brief Vector que contem todos os vértices da malha
     */
    std::vector<core::Vertex *> vertices;
    /**
     * @brief Vector que contem os índices dos vértices das faces
     *
     * @note Usado apenas na criação da malha (indexação dos vértices) e na exportação para JSON
     */
    std::vector<std::vector<int>> index_vertices;
    /**
     * @brief Vector que contem todas as faces da malha
     */
    std::vector<core::Face *> faces;
    /**
     * @brief Vector que contem todas as meias arestas da malha
     */
    std::vector<core::HalfEdge *> half_edges;
    /**
     * @brief Número de faces da malha
     */
    int num_faces;
    /**
     * @brief Texto identificador da malha
     */
    std::string id;
    /**
     * @brief Nome do objeto
     */
    std::string name;
    /**
     * @brief Flag para indicar se o objeto esta selecionado
     */
    bool selected;
    /**
     * @brief Mapa de vértices da malha
     *
     * @note Usado apenas na criação da malha
     */
    std::map<std::string, core::HalfEdge *> half_edges_map;

  public:
    // Atributos da malha
    /**
     * @brief Material do objeto
     *
     * @note Conteúdo do material: Cor difusa, Cor especular e Brilho
     */
    models::Material material;
    /**
     * @brief Flag para indicar se o objeto está dentro do plano near e far
     */
    bool is_visible = true;
    /**
     * @brief Posição do objeto no espaço
     *
     * @note A posição do objeto é dada em coordenadas do SRU (Sistema de Referência do Universo)
     * @note Utilizado para a transformação de objetos
     */
    core::Vector3 position = {0, 0, 0};
    /**
     * @brief Rotação do objeto no espaço
     *
     * @note A rotação do objeto é dada em coordenadas do SRU (Sistema de Referência do Universo)
     * @note Utilizado para a transformação de objetos
     */
    core::Vector3 rotation = {0, 0, 0};
    /**
     * @brief Escala do objeto no espaço
     *
     * @note A escala do objeto é dada em coordenadas do SRU (Sistema de Referência do Universo)
     * @note Utilizado para a transformação de objetos
     */
    core::Vector3 scale = {1, 1, 1};

    // Constructors and Destructors
    Mesh();
    Mesh(std::vector<core::Vertex *> vertexes, std::vector<std::vector<int>> faces, std::string id);
    ~Mesh();

    // Copy constructor
    Mesh(const Mesh &mesh);

    // Copy assignment
    Mesh &operator=(const Mesh &mesh);

    // Getters and Setters
    std::vector<core::Vertex *> getVertices() const;
    std::vector<core::Face *> getFaces() const;
    std::vector<core::HalfEdge *> getHalfEdges() const;
    int getNumFaces() const;
    std::string getId() const;
    std::string getName() const;
    bool isSelected() const;

    void setVertices(const std::vector<core::Vertex *> vertices);
    void setFaces(const std::vector<core::Face *> faces);
    void setHalfEdges(const std::vector<core::HalfEdge *> half_edges);
    void setNumFaces(int num_faces);
    void setId(const std::string &id);
    void setName(const std::string &name);
    void setSelected(bool selected);
    void setMaterial(models::Material material);

    // Functions
    void createMesh(std::vector<std::vector<int>> index_faces);
    void createMesh(const std::string &filename);
    void createMesh(const std::vector<core::Vertex> &vertices, std::vector<std::vector<int>> index_faces);
    core::Vector4 getBox(bool screen_coordinates);                // x = min_x, y = min_y, z = max_x, w = max_y
    std::vector<core::Vector3> getBox3D(bool screen_coordinates); // v[0] = min, v[1] = max
    void determineNormals();
    void determineNormalsByAverage();
    core::Vector3 getCentroidByMean();
    core::Vector3 getCentroidByWrapBox();

    void clearMesh();
    core::HalfEdge *addEdge(core::Vertex *vertex1, core::Vertex *vertex2);
    core::Face *addFaceByHalfEdges(std::vector<core::HalfEdge *> half_edges);
    core::Face *addFaceByVertices(std::vector<core::Vertex *> vertexes);
    core::HalfEdge *findEdge(core::Vertex *vertex1, core::Vertex *vertex2);

    bool isOutsideViewport(core::Vector2 viewport_min, core::Vector2 viewport_max);

    json to_json();
    void from_json(json json_data);
  };
} // namespace models