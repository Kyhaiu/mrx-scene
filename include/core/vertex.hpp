/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxvertex v1.0 - Estrutura de dados e funções básicas para trabalhar com vértices
 *
 *   CONVENTIONS: (Convenções)
 *   - As funções são sempre auto-contidas, nenhuma função usa outra função mrxvertex dentro,
 *     o código necessário é re-implementado diretamente dentro.
 *       * Esta é uma maneira de evitar qualquer dependência e evitar a sobrecarga de chamadas
 *   - Os parâmetros de entrada das funções são sempre recebidos por valor.
 *   - As funções sempre usam uma variável "result" para retorno
 *   - Este módulo não deve ser usado como tipo de dados, para isto utilize o módulo mrxvector
 *   - As classes respeitam a convenção de nomes PascalCase
 *   - As funções respeitam a convenção de nomes camelCase
 *   - As variáveis respeitam a convenção de nomes snake_case
 *   - A classe utiliza a regra de 3 (construtor, destrutor e operador de atribuição)
 *
 *   IDIOM: ENGLISH
 *
 *   mrxvertex v1.0 - Data structure and basic functions to work with vertices
 *
 *   CONVENTIONS:
 *   - Functions are always self-contained, no function use another mrxvertex function inside,
 *     required code is directly re-implemented inside.
 *       - This is a way to avoid any dependency and avoid calling overhead
 *   - Functions input parameters are always received by value.
 *   - Functions use always a "result" variable for return
 *   - This module should not be used as a data type, for this use the mrxvector module
 *   - Classes respect PascalCase naming convention
 *   - Functions respect camelCase naming convention
 *   - Variables respect snake_case naming convention
 *   - The class uses the rule of 3 (constructor, destructor and assignment operator)
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <core/vector.hpp> - Required for: Vector2, Vector3 and Vector4
 *      <math/math.hpp> - Required for: Vector3Normalize
 *      <string> - Required for: std::string
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

#include <string>
#include <core/common.hpp>
#include <math/math.hpp>

namespace core
{
  class Vertex
  {

  private:
    // Attributes

    /**
     * @brief Vetor 4D da classe Vertex que contem as coordenadas do vértice
     */
    MRX_VECTOR4_TYPE vector;

    /**
     * @brief Vetor 3D que contem as coordenadas convertidas para o sistema da tela
     */
    MRX_VECTOR3_TYPE _vector_screen;

    /**
     * @brief Flag para indicar se o vértice foi clipado
     */
    bool clipped = false;

    /**
     * @brief Ponteiro para a meia aresta que tem o vértice como origem
     */
    core::HalfEdge *half_edge;

    /**
     * @brief Identificador do vértice
     */
    std::string id;

    /**
     * @brief Vetor normal médio do vértice
     *
     * @note Este vetor é calculado a partir das normais das faces que compartilham o vértice
     * @note O vetor já está em sua forma normalizada
     */
    MRX_VECTOR3_TYPE normal;

  public:
    // Constructors and destructors
    Vertex();
    Vertex(float x, float y, float z, float w, core::HalfEdge *half_edge = nullptr, std::string id = "");
    ~Vertex();

    // Copy constructor
    Vertex(const Vertex &vertex);

    // Copy assignment operator
    Vertex &operator=(const Vertex &vertex);

    // Getters and setters
    MRX_VECTOR4_TYPE getVector() const;
    void setVector(const MRX_VECTOR4_TYPE &vector);

    MRX_VECTOR3_TYPE getVectorScreen();
    void setVectorScreen(const MRX_VECTOR3_TYPE &vector);

    bool getClipped() const;
    void setClipped(bool clipped);

    float getX(bool screen_coord = false) const;
    void setX(float x, bool screen_coord = false);

    float getY(bool screen_coord = false) const;
    void setY(float y, bool screen_coord = false);

    float getZ(bool screen_coord = false) const;
    void setZ(float z, bool screen_coord = false);

    float getW(bool screen_coord = false) const;
    void setW(float w, bool screen_coord = false);

    core::HalfEdge *getHalfEdge() const;
    void setHalfEdge(core::HalfEdge *half_edge);

    std::string getId() const;
    void setId(const std::string &id);

    MRX_VECTOR3_TYPE getNormal() const;
    void setNormal(const MRX_VECTOR3_TYPE &normal);

    // Methods
    MRX_VECTOR4_TYPE normalize() const;
    core::float4 toArray() const;
    friend std::ostream &operator<<(std::ostream &os, const Vertex &vertex);
  };
} // namespace core