#include <models/camera.hpp>

namespace models
{
  /**
   * @brief Função que retorna a posição da câmera
   *
   * @param camera Ponteiro para a câmera
   *
   * @return Vetor 3D com as coordenadas da posição da câmera
   */
  core::Vector3 getCameraPosition(models::Camera3D *camera)
  {
    return camera->position;
  }

  /**
   * @brief Função que retorna o ponto para onde a câmera está olhando
   *
   * @param camera Ponteiro para a câmera
   *
   * @return Vetor 3D com as coordenadas do ponto para onde a câmera está olhando
   */
  core::Vector3 getCameraTarget(models::Camera3D *camera)
  {
    return camera->target;
  }

  /**
   * @brief Função que retorna o vetor que representa a direção para cima da câmera
   *
   * @param camera Ponteiro para a câmera
   *
   * @return Vetor 3D com as coordenadas do vetor que representa a direção para cima da câmera
   */
  core::Vector3 getCameraUp(models::Camera3D *camera)
  {
    return camera->up;
  }

  /**
   * @brief Função que retorna o vetor que representa a direção para frente da câmera
   *
   * @param camera Ponteiro para a câmera
   *
   * @return Vetor 3D com as coordenadas do vetor que representa a direção para frente da câmera
   */
  core::Vector3 getCameraForward(models::Camera3D *camera)
  {
    return math::Vector3Normalize(math::Vector3Subtract(camera->target, camera->position));
  }

  /**
   * @brief Função que move a câmera para frente
   *
   * @param camera Ponteiro para a câmera
   * @param distance Distância que a câmera deve se mover
   * @param moveInWorldPlane Flag que indica se a câmera deve se mover no plano do mundo
   */
  void CameraMoveForward(models::Camera3D *camera, float distance, bool moveInWorldPlane)
  {
    core::Vector3 forward = models::getCameraForward(camera);

    if (moveInWorldPlane)
    {
      // Projetar o vetor no plano XZ (plano de movimento)
      forward.y = 0;
      forward = math::Vector3Normalize(forward);
    }

    forward = math::Vector3Scale(forward, distance);

    camera->position = math::Vector3Add(camera->position, forward);
    camera->target = math::Vector3Add(camera->target, forward);
  }

  /**
   * @brief Função que move a câmera para cima
   *
   * @param camera Ponteiro para a câmera
   * @param distance Distância que a câmera deve se mover
   */
  void CameraMoveUp(models::Camera3D *camera, float distance)
  {
    core::Vector3 up = models::getCameraUp(camera);
    up = math::Vector3Scale(up, distance);

    camera->position = math::Vector3Add(camera->position, up);
    camera->target = math::Vector3Add(camera->target, up);
  }

  /**
   * @brief Função que move a câmera para a direita
   *
   * @param camera Ponteiro para a câmera
   * @param distance Distância que a câmera deve se mover
   * @param moveInWorldPlane Flag que indica se a câmera deve se mover no plano do mundo
   */
  void CameraMoveRight(models::Camera3D *camera, float distance, bool moveInWorldPlane)
  {
    core::Vector3 forward = models::getCameraForward(camera);
    core::Vector3 right = math::Vector3CrossProduct(forward, camera->up);
    right = math::Vector3Normalize(right);

    if (moveInWorldPlane)
    {
      // Projetar o vetor no plano XZ (plano de movimento)
      right.y = 0;
      right = math::Vector3Normalize(right);
    }

    right = math::Vector3Scale(right, distance);

    camera->position = math::Vector3Add(camera->position, right);
    camera->target = math::Vector3Add(camera->target, right);
  }

  /**
   * @brief Função que move a posição da camera para perto/longe da câmera
   *
   * @param camera Ponteiro para a câmera
   * @param target Vetor 3D com as coordenadas do ponto alvo
   */
  void CameraMoveToTarget(models::Camera3D *camera, float target)
  {
    float distance = math::Vector3Distance(camera->position, camera->target);

    distance += target;

    if (distance)
    {
      distance = 0.001f;
    }

    core::Vector3 forward = models::getCameraForward(camera);
    camera->position = math::Vector3Add(camera->target, math::Vector3Scale(forward, -distance));
  }

  /**
   * @brief Função que rotaciona a câmera em torno do vetor up
   *
   * @param camera Ponteiro para a câmera
   * @param angle Ângulo de rotação
   * @param rotateAroundTarget Flag que indica se a câmera deve rotacionar em torno do ponto alvo
   *
   * @note O angulo deve ser passado em radianos
   * @note Se rotateAroundTarget for falso, a câmera rotaciona em torno de si mesma
   */
  void CameraYaw(models::Camera3D *camera, float angle, bool rotateAroundTarget)
  {
    core::Vector3 up = models::getCameraUp(camera);
    core::Vector3 targetPosition = math::Vector3Subtract(camera->target, camera->position);

    // Rotacionar o vetor target em torno do vetor up
    targetPosition = math::Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
      camera->target = math::Vector3Subtract(camera->target, targetPosition);
    }
    else
    {
      camera->target = math::Vector3Add(camera->position, targetPosition);
    }
  }

  /**
   * @brief Função que rotaciona a câmera em torno do vetor right, pitch é "olhar para cima ou para baixo"
   *
   * @param camera Ponteiro para a câmera
   * @param angle Ângulo de rotação
   * @param lockView Flag que previne a super rotação da câmera
   * @param rotateAroundTarget Flag que indica se a câmera deve rotacionar em torno do ponto alvo ou em torno de si mesma
   * @param rotateUp Flag que indica se a câmera deve rotacionar o vetor up (tipicamente usado em camera livre)
   */
  void CameraPitch(models::Camera3D *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp)
  {
    core::Vector3 up = math::Vector3Subtract(camera->target, camera->position);

    core::Vector3 targetPosition = math::Vector3Subtract(camera->target, camera->position);

    if (lockView)
    {
      // Nesse modo de câmera, nós restringimos o angulo
      // para permitir apenas visões de 90 graus para cima e para baixo

      // Clamp view up
      float maxAngleUp = math::Vector3Angle(up, targetPosition);
      maxAngleUp -= 0.001f; // Remove um pouco de margem para evitar problemas de precisão
      if (angle > maxAngleUp)
      {
        angle = maxAngleUp;
      }

      float maxAngleDown = math::Vector3Angle(math::Vector3Negate(up), targetPosition);
      maxAngleDown *= -1.0f;  // Inverte o sinal
      maxAngleDown += 0.001f; // Adiciona um pouco de margem para evitar problemas de precisão
      if (angle < maxAngleDown)
      {
        angle = maxAngleDown;
      }
    }

    core::Vector3 forward = models::getCameraForward(camera);
    core::Vector3 right = math::Vector3CrossProduct(forward, camera->up);
    right = math::Vector3Normalize(right);

    // Rotacionar o vetor target em torno do vetor right
    targetPosition = math::Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget)
    {
      camera->target = math::Vector3Subtract(camera->target, targetPosition);
    }
    else
    {
      camera->target = math::Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp)
    {
      camera->up = math::Vector3RotateByAxisAngle(camera->up, right, angle);
    }
  }

  /**
   * @brief Função que rotaciona a câmera em torno do vetor forward
   *
   * @param camera Ponteiro para a câmera
   * @param angle Ângulo de rotação
   *
   * @note Roll é "virar a sua cabeça de lado para a direita ou para a esquerda"
   */
  void CameraRoll(models::Camera3D *camera, float angle)
  {
    core::Vector3 forward = models::getCameraForward(camera);

    // Rotacionar o vetor up em torno do vetor forward
    camera->up = math::Vector3RotateByAxisAngle(camera->up, forward, angle);
  }

  /**
   * @brief Função que cria uma câmera 3D
   *
   * @param position Vetor 3D com as coordenadas da posição da câmera
   * @param target Vetor 3D com as coordenadas do ponto para onde a câmera está olhando
   * @param up Vetor 3D com as coordenadas do vetor que representa a direção para cima da câmera
   * @param d Distância do plano de projeção
   *
   * @return Câmera 3D
   */
  models::Camera3D *CreateCamera3D(core::Vector3 position, core::Vector3 target, core::Vector3 up, float d)
  {
    models::Camera3D *result = new models::Camera3D();

    result->position = position;
    result->target = target;
    result->up = up;
    result->d = d;
    result->lockView = false;
    result->rotateAroundTarget = false;
    result->rotateUp = false;

    return result;
  }

  std::ostream &operator<<(std::ostream &os, const models::Camera3D &camera)
  {
    os << "Camera3D: {" << std::endl;
    os << "  position: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;
    os << "  target: (" << camera.target.x << ", " << camera.target.y << ", " << camera.target.z << ")" << std::endl;
    os << "  up: (" << camera.up.x << ", " << camera.up.y << ", " << camera.up.z << ")" << std::endl;
    os << "  d: " << camera.d << std::endl;
    os << "}";

    return os;
  }
} // namespace