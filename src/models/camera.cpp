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
    return math::Vector3Normalize(camera->up);
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
   * @brief Função que retorna o vetor que representa a direção para a direita da câmera
   *
   * @param camera Ponteiro para a câmera
   *
   * @return Vetor 3D com as coordenadas do vetor que representa a direção para a direita da câmera
   */
  core::Vector3 getCameraRight(models::Camera3D *camera)
  {
    core::Vector3 forward = models::getCameraForward(camera);
    core::Vector3 right = math::Vector3CrossProduct(forward, camera->up);

    return math::Vector3Normalize(right);
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
    core::Vector3 right = models::getCameraRight(camera);

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
   * @brief Camera orbital, apenas orbita o objeto
   *
   * @param camera Ponteiro para a câmera
   * @param orbitalSpeed Velocidade orbital da câmera
   */
  void CameraOrbital(models::Camera3D *camera, float orbitalSpeed)
  {

    core::Matrix rotation = math::MatrixRotate(math::Vector3Normalize({0, 1, 0}), orbitalSpeed);
    core::Vector3 view = math::Vector3Subtract(camera->position, camera->target);
    view = math::Vector3Transform(view, rotation);
    camera->position = math::Vector3Add(camera->target, view);
  }

  /**
   * @brief Função que rotaciona a câmera em torno do ponto alvo
   *
   * @param camera Ponteiro para a câmera
   * @param deltaAngleX Ângulo de rotação em torno do eixo X
   * @param deltaAngleY Ângulo de rotação em torno do eixo Y
   *
   * @ref https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
   */
  void CameraArcball(models::Camera3D *camera, float deltaAngleX, float deltaAngleY)
  {
    core::Vector3 camera_position = camera->position;
    core::Vector3 camera_direction = math::Vector3Subtract(camera_position, camera->target);

    // Rotaciona a camera em torno do eixo X
    camera_position = math::Vector3Add(camera->target, math::Vector3Transform(camera_direction, math::MatrixRotate({1, 0, 0}, deltaAngleX)));
    camera_direction = math::Vector3Subtract(camera_position, camera->target);

    // Rotaciona a camera em torno do eixo Y
    camera_position = math::Vector3Add(camera->target, math::Vector3Transform(camera_direction, math::MatrixRotate({0, 1, 0}, deltaAngleY)));

    camera->position = camera_position;
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

  /**
   * @brief Função que retorna as coordenadas esféricas da câmera
   *
   * @param camera Posição da câmera
   *
   * @return core::Vector3 - Vetor 3D com as coordenadas esféricas da câmera
   */
  core::Vector3 GetSphericalCoordinates(core::Vector3 camera)
  {
    core::Vector3 result = {0};

    float r = sqrtf(powf(camera.x, 2) + powf(camera.y, 2) + powf(camera.z, 2));
    float phi = atan2f(camera.z / camera.y, camera.x);
    float theta = acosf(camera.y / r);

    if (camera.x < 0)
      phi += PI;

    result.x = r;
    result.y = phi;
    result.z = theta;

    return result;
  }

  /**
   * @brief Função que retorna as coordenadas cartesianas da câmera
   *
   * @param camera Ponteiro para a câmera
   *
   * @return core::Vector3 - Vetor 3D com as coordenadas cartesianas da câmera
   */
  core::Vector3 GetCartesianCoordinates(core::Vector3 camera)
  {
    core::Vector3 result = {0};

    result.x = camera.x * cosf(camera.z) * cosf(camera.y);
    result.y = camera.x * sinf(camera.z);
    result.z = camera.x * cosf(camera.z) * sinf(camera.y);

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