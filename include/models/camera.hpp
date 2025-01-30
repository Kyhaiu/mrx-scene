/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxcamera v1.0 - Modelo básico de camera
 *
 *   CONVENÇÕES:
 *    - A camera é uma struct ao invés de uma classe para facilitar a manipulação dos dados
 *    - Os métodos que manipulam a camera são funções que recebem um ponteiro para a camera
 *    - O VRP (View Reference Point) é a posição da camera (camera->position)
 *    - O P (Point) é o ponto para onde a camera está olhando (camera->target)
 *    - O VUP (View Up) é o vetor que representa a direção para cima da camera (camera->up)
 *    - As operações de rotação são realizadas em torno do eixo Y (yaw), X (pitch) e Z (roll)
 *
 *   IDIOM: ENGLISH
 *
 *   mrxcamera v1.0 - Basic camera model
 *
 *   CONVENTIONS:
 *    - The camera is a struct instead of a class to facilitate data manipulation
 *    - The methods that manipulate the camera are functions that receive a pointer to the camera
 *    - The VRP (View Reference Point) is the camera position (camera->position)
 *    - The P (Point) is the point where the camera is looking (camera->target)
 *    - The VUP (View Up) is the vector that represents the direction upwards of the camera (camera->up)
 *    - The rotation operations are performed around the Y (yaw), X (pitch) and Z (roll) axes
 *
 *   CONFIGURATION:
 *
 *   DEPENDENCIES:
 *    <core/vector.hpp> - Required for: types core::Vector3
 *    <math/math.hpp>   - Required for:
 *      - math::Vector3Normalize
 *      - math::Vector3Subtract
 *      - math::Vector3Add
 *      - math::Vector3CrossProduct
 *      - math::Vector3Scale
 *      - math::Vector3Distance
 *      - math::Vector3RotateByAxisAngle
 *      - math::Vector3Angle
 *      - math::Vector3Negate
 *      - math::MatrixRotation
 *    <iostream>        - Required for: std::ostream, std::endl
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

#include <math/math.hpp>
#include <core/vector.hpp>

#include <utils/nlohmann/json.hpp>

#include <math.h>
#include <iostream>

using json = nlohmann::json;

namespace models
{
  typedef struct Camera3D
  {
    /**
     * @brief Vetor 3D que contem as coordenadas da posição da câmera
     */
    core::Vector3 position;
    /**
     * @brief Vetor 3D que contem as coordenadas do ponto para onde a câmera está olhando
     */
    core::Vector3 target;
    /**
     * @brief Vetor 3D que contem as coordenadas do vetor que representa a direção para cima da câmera
     */
    core::Vector3 up;
    /**
     * @brief Distância do plano de projeção
     */
    float d;
    /**
     * @brief Distância do plano de projeção mais próximo
     */
    float near;
    /**
     * @brief Distância do plano de projeção mais distante
     */
    float far;
    /**
     * @brief Flag que indica se a câmera deve rotacionar em torno do ponto alvo
     */
    bool rotateAroundTarget;
    /**
     * @brief Flag que previne a super rotação da câmera
     */
    bool lockView;
    /**
     * @brief Flag que indica se a câmera deve rotacionar o vetor up (tipicamente usado em camera livre)
     */
    bool rotateUp;

    json to_json()
    {
      return json{
          {"position", position.to_json()},
          {"target", target.to_json()},
          {"up", up.to_json()},
          {"d", d},
          {"far", far},
          {"near", near}};
    }

    static Camera3D *from_json(json json_data)
    {
      return new Camera3D{
          core::Vector3::from_json(json_data["position"]),
          core::Vector3::from_json(json_data["target"]),
          core::Vector3::from_json(json_data["up"]),
          json_data["d"],
          json_data["near"],
          json_data["far"],
          false, false, false};
    }
  } Camera3D;

  //------------------------------------------------------------------------------------------------
  // Module Functions Declaration
  //------------------------------------------------------------------------------------------------
  core::Vector3 getCameraPosition(Camera3D *camera);
  core::Vector3 getCameraTarget(Camera3D *camera);
  core::Vector3 getCameraUp(Camera3D *camera);
  core::Vector3 getCameraForward(Camera3D *camera);
  core::Vector3 getCameraRight(Camera3D *camera);

  void CameraMoveForward(Camera3D *camera, float distance, bool moveInWorldPlane);
  void CameraMoveUp(Camera3D *camera, float distance);
  void CameraMoveRight(Camera3D *camera, float distance, bool moveInWorldPlane);
  void CameraMoveToTarget(Camera3D *camera, float target);

  void CameraYaw(Camera3D *camera, float angle, bool rotateAroundTarget);
  void CameraPitch(Camera3D *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
  void CameraRoll(Camera3D *camera, float angle);

  void CameraOrbital(Camera3D *camera, float orbitalSpeed);
  void CameraArcball(Camera3D *camera, float deltaAngleX, float deltaAngleY);

  models::Camera3D *CreateCamera3D(core::Vector3 position, core::Vector3 target, core::Vector3 up, float d, float near, float far);

  core::Vector3 GetSphericalCoordinates(core::Vector3 camera);
  core::Vector3 GetCartesianCoordinates(core::Vector3 camera);

  std::ostream &operator<<(std::ostream &os, const Camera3D &camera);

} // namespace core