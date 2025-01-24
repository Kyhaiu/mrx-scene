/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxscene v1.0 - Estrutura de dados e funções básicas para trabalhar com cenas
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *     - As funções sempre usam uma variável "result" para retorno
 *
 *   IDIOM: ENGLISH
 *
 *   mrxscene v1.0 -
 *
 *   CONVENTIONS:
 *     - The functions always have a @brief, @param and @return description in the .cpp file
 *     - The functions always use a "result" variable for return
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *
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

#include <limits>

#include <vector>
#include <iostream>

#include <models/mesh.hpp>
#include <models/camera.hpp>
#include <math/pipeline.hpp>
#include <models/colors.hpp>
#include <models/light.hpp>
#include <utils/nlohmann/json.hpp>
#include <utils/utils.hpp>

using json = nlohmann::json;

namespace models
{
#define SANTA_CATARINA_PIPELINE 0
#define SMITH_PIPELINE 1

#define FOLEY_UNIT_NORMAL_VECTOR 0
#define CONCI_UNIT_NORMAL_VECTOR 1

#define CENTROID_BY_MEAN 0
#define CENTROID_BY_WRAP_BOX 1

  class Scene
  {
  private:
    /**
     * @brief Vector que contem todos os objetos da cena
     */
    std::vector<models::Mesh *> objects;
    /**
     * @brief Câmera da cena
     */
    models::Camera3D *camera;
    /**
     * @brief Objeto selecionado da cena
     */
    models::Mesh *selected_object;
    /**
     * @brief coordenadas minímas x e y da viewport (tela)
     */
    core::Vector2 min_viewport;
    /**
     * @brief coordenadas máximas x e y da viewport (tela)
     */
    core::Vector2 max_viewport;
    /**
     * @brief coordenadas minímas x e y da janela
     */
    core::Vector2 min_window;
    /**
     * @brief coordenadas máximas x e y da janela
     */
    core::Vector2 max_window;
    /**
     * @brief Flag que indica se a cena possui reflexão
     */
    bool reflection = false;
    /**
     * @brief Flag que indica se a cena foi processada
     */
    bool processed = false;

  public:
    /**
     * @brief Buffer de profundidade
     */
    std::vector<std::vector<float>> z_buffer;
    /**
     * @brief Buffer de cores
     */
    std::vector<std::vector<models::Color>> color_buffer;
    /**
     * @brief Luz global da cena
     *
     */
    models::Light global_light;
    /**
     * @brief Lampadas omni da cena
     *
     * @note Omni refere-se a lampada omnidirecional que tem as mesmas propriedades de cor em todas as direções
     */
    std::vector<models::Omni> omni_lights;
    /**
     * @brief Flag que determina qual modelo de iluminação será utilizado
     *
     * @note 0 - Flat Shading (Padrão)
     * @note 1 - Gouraud Shading
     * @note 2 - Phong Shading
     */
    int lighting_model = FLAT_SHADING;
    /**
     * @brief Flag que determina qual modelo de pipeline será utilizado
     *
     * @note 0 - Adair Santa Catarina (Padrão)
     * @note 1 - Alvy Ray Smith
     */
    int pipeline_model = SANTA_CATARINA_PIPELINE;
    /**
     * @brief Flag que determina qual algoritmo de vetor normal unitário será utilizado
     *
     * @note 0 - Foley (Padrão)
     * @note 1 - Conci, Azevedo e Leta
     */
    int normal_algorithm = FOLEY_UNIT_NORMAL_VECTOR;
    /**
     * @brief Flag que determina qual algoritmo de cálculo do centroide será utilizado
     *
     * @note 0 - Média dos vértices (Padrão)
     * @note 1 - Caixa envolvente
     */
    int centroid_algorithm = CENTROID_BY_MEAN;

    // Construtor and Destrutor
    Scene();
    Scene(models::Camera3D *camera, std::vector<models::Mesh *> objects, core::Vector2 min_viewport, core::Vector2 max_viewport, core::Vector2 min_window, core::Vector2 max_window);
    ~Scene();

    // Getters and Setters
    models::Camera3D *getCamera();
    std::vector<models::Mesh *> getObjects();
    models::Mesh *getSelectedObject();
    core::Vector2 getMinViewport();
    core::Vector2 getMaxViewport();
    core::Vector2 getMinWindow();
    core::Vector2 getMaxWindow();
    bool getReflection();
    bool getProcessed();
    void setCamera(models::Camera3D *camera);
    void setObjects(std::vector<models::Mesh *> objects);
    void setSelectedObject(models::Mesh *selected_object);
    void setMinViewport(core::Vector2 min_viewport);
    void setMaxViewport(core::Vector2 max_viewport);
    void setMinWindow(core::Vector2 min_window);
    void setMaxWindow(core::Vector2 max_window);
    void setReflection(bool reflection);
    void setProcessed(bool processed);

    // Functions
    void addObject(models::Mesh *object);
    void removeObject(models::Mesh *object);
    void rasterize_adair_pipeline();
    void rasterize_smith_pipeline();
    void selectObject(int x, int y);
    void deselectObject();
    void moveCamera(int x, int y);
    void initializeBuffers();
    json to_json();
    void from_json(json json_data);

    // Individual Object Transformations
    void translateObject(core::Vector3 translation);
    void scaleObject(core::Vector3 scale);
    void rotateObject(core::Vector3 axis, float angle);
    void rotateObject(core::Vector3 angle);
  };
} // namespace models