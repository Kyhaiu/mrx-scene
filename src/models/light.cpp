#include <models/light.hpp>

namespace models
{
  //-------------------------------------------------------------------------------------------------
  // Funções
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Luz orbital, apenas faz a luz se mover em torno do objeto
   *
   * @param omni Referência para a luz omnidirecional
   * @param orbitalSpeed Velocidade orbital da luz
   *
   * @note A luz rotaciona em torno do eixo Y
   * @note A luz rotaciona em torno do centro da cena (0, 0, 0)
   */
  void LightOrbital(models::Omni *omni, float orbitalSpeed)
  {
    // (0, 1, 0) é o vetor up (eixo de rotação)
    core::Matrix rotation = math::MatrixRotate(math::Vector3Normalize({0, 1, 0}), orbitalSpeed);
    // (0, 0, 0) é o centro da cena já que a luz não possui ponto focal (raio de visão)
    core::Vector3 view = math::Vector3Subtract(omni->position, {0, 0, 0});
    view = math::Vector3Transform(view, rotation);
    omni->position = math::Vector3Add({0, 0, 0}, view);
  }

  /**
   * @brief Calcula a iluminação de um objeto utilizando o modelo de iluminação constante
   *
   * @param light Luz ambiente da cena
   * @param omni Lampa omnidirecionais
   * @param centroid Centroide da face
   * @param eye Posição do observador (câmera)
   * @param material Material do objeto
   */
  models::Color FlatShading(const models::Light &light, const std::vector<models::Omni> &omni, const core::Vector3 &centroid, const core::Vector3 &face_normal, const core::Vector3 &eye, const models::Material &material)
  {
    models::Color ambient_illumination = models::BLACK;
    models::Color diffuse_illumination = models::BLACK;
    models::Color specular_illumination = models::BLACK;

    // Passo 1: Calcular a iluminação ambiente
    ambient_illumination.r = static_cast<models::Uint8>(math::Clamp(light.intensity.r * material.ambient.r, 0, 255));
    ambient_illumination.g = static_cast<models::Uint8>(math::Clamp(light.intensity.g * material.ambient.g, 0, 255));
    ambient_illumination.b = static_cast<models::Uint8>(math::Clamp(light.intensity.b * material.ambient.b, 0, 255));

    // pre computar o vetor S (direção do observador) já que ele é constante
    core::Vector3 S = math::Vector3Normalize(math::Vector3Subtract(eye, centroid));

    // Para cada fonte de luz na cena
    for (auto lamp : omni)
    {
      // Passo 2: Calcular a iluminação difusa
      // Vetor da luz (direção da luz)
      core::Vector3 L = math::Vector3Normalize(math::Vector3Subtract(lamp.position, centroid));

      float cos_theta = math::Vector3DotProduct(face_normal, L);

      models::ColorChannels kd = material.diffuse;

      if (cos_theta > 0)
      {
        diffuse_illumination.r = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.r + (lamp.intensity.r * kd.r * cos_theta), 0, 255));
        diffuse_illumination.g = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.g + (lamp.intensity.g * kd.g * cos_theta), 0, 255));
        diffuse_illumination.b = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.b + (lamp.intensity.b * kd.b * cos_theta), 0, 255));
      }

      // Passo 3: Calcular a iluminação especular

      // Vetor da reflexão da luz (R = (2N.L). N - L)
      // N = normal da face
      core::Vector3 R = math::Vector3Subtract(math::Vector3MultiplyValue(face_normal, 2 * cos_theta), L);

      float cos_alpha = math::Vector3DotProduct(R, S);

      models::ColorChannels ks = material.specular;
      float n = material.shininess;

      if (cos_alpha > 0)
      {
        specular_illumination.b = static_cast<models::Uint8>(math::Clamp(specular_illumination.r + (lamp.intensity.r * ks.b * pow(cos_alpha, n)), 0, 255));
        specular_illumination.r = static_cast<models::Uint8>(math::Clamp(specular_illumination.g + (lamp.intensity.g * ks.r * pow(cos_alpha, n)), 0, 255));
        specular_illumination.g = static_cast<models::Uint8>(math::Clamp(specular_illumination.b + (lamp.intensity.b * ks.g * pow(cos_alpha, n)), 0, 255));
      }
    }

    models::Color color = models::BLACK;

    ambient_illumination = {0, 0, 0, 255};
    // diffuse_illumination = models::BLACK;
    specular_illumination = {0, 0, 0, 255};

    // Passo 4: Calcular a cor final
    color.r = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.r + diffuse_illumination.r + specular_illumination.r), 0, 255));
    color.g = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.g + diffuse_illumination.g + specular_illumination.g), 0, 255));
    color.b = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.b + diffuse_illumination.b + specular_illumination.b), 0, 255));

    return color;
  }

  /**
   * @brief Calcula a iluminação de um objeto utilizando o modelo de iluminação de Gouraud
   *
   * @param light Luz ambiente da cena
   * @param omni Vetor de Lampa omnidirecionais
   * @param vertexes Vértice da face e Normal médio do vértice
   * @param eye Posição do observador (câmera)
   * @param material Material do objeto
   *
   * @return models::Color Cor do vértice
   */
  models::Color GouraudShading(const models::Light &light, const std::vector<models::Omni> &omni, const std::pair<core::Vector3, core::Vector3> &vertex, const core::Vector3 &eye, const models::Material &material)
  {
    return FlatShading(light, omni, vertex.first, vertex.second, eye, material);
  }

  /**
   * @brief Calcula a iluminação de um objeto utilizando o modelo de iluminação de Phong
   *
   * @param light Luz ambiente da cena
   * @param omni Vetor de Lampa omnidirecionais
   * @param centroid Centroide da face
   * @param pixel Posição do pixel
   * @param pixel_normal Normal do pixel
   * @param eye Posição do observador (câmera)
   * @param material Material do objeto
   * @return models::Color Cor do pixel
   */
  models::Color PhongIllumination(const models::Light &light, const std::vector<models::Omni> &omni, const core::Vector3 &centroid, const core::Vector3 &pixel, const core::Vector3 &pixel_normal, const core::Vector3 &eye, const models::Material &material)
  {
    models::Color ambient_illumination = models::BLACK;
    models::Color diffuse_illumination = models::BLACK;
    models::Color specular_illumination = models::BLACK;

    core::Vector3 pixel_normal_normalized = math::Vector3Normalize(pixel_normal);

    // Passo 1: Calcular a iluminação ambiente
    ambient_illumination.r = static_cast<models::Uint8>(math::Clamp(light.intensity.r * material.ambient.r, 0, 255));
    ambient_illumination.g = static_cast<models::Uint8>(math::Clamp(light.intensity.g * material.ambient.g, 0, 255));
    ambient_illumination.b = static_cast<models::Uint8>(math::Clamp(light.intensity.b * material.ambient.b, 0, 255));

    // pre computar o vetor S (direção do observador) já que ele é constante
    core::Vector3 S = math::Vector3Normalize(math::Vector3Subtract(eye, centroid));

    // Para cada fonte de luz na cena
    for (auto lamp : omni)
    {
      // Passo 2: Calcular a iluminação difusa
      // Vetor da luz (direção da luz)
      core::Vector3 L = math::Vector3Normalize(math::Vector3Subtract(lamp.position, centroid));

      float cos_theta = math::Vector3DotProduct(pixel_normal_normalized, L);

      models::ColorChannels kd = material.diffuse;

      if (cos_theta > 0)
      {
        diffuse_illumination.r = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.r + (lamp.intensity.r * kd.r * cos_theta), 0, 255));
        diffuse_illumination.g = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.g + (lamp.intensity.g * kd.g * cos_theta), 0, 255));
        diffuse_illumination.b = static_cast<models::Uint8>(math::Clamp(diffuse_illumination.b + (lamp.intensity.b * kd.b * cos_theta), 0, 255));

        // Passo 3: Calcular a iluminação especular
        core::Vector3 LS = math::Vector3Add(L, S);
        core::Vector3 H = math::Vector3Normalize(LS);

        float cos_alpha = math::Vector3DotProduct(pixel_normal_normalized, H);

        models::ColorChannels ks = material.specular;
        float n = material.shininess;

        if (cos_alpha > 0)
        {
          specular_illumination.b = static_cast<models::Uint8>(math::Clamp(specular_illumination.r + (lamp.intensity.r * ks.b * pow(cos_alpha, n)), 0, 255));
          specular_illumination.r = static_cast<models::Uint8>(math::Clamp(specular_illumination.g + (lamp.intensity.g * ks.r * pow(cos_alpha, n)), 0, 255));
          specular_illumination.g = static_cast<models::Uint8>(math::Clamp(specular_illumination.b + (lamp.intensity.b * ks.g * pow(cos_alpha, n)), 0, 255));
        }
      }
    }

    models::Color color = models::BLACK;

    // Passo 4: Calcular a cor final
    color.r = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.r + diffuse_illumination.r + specular_illumination.r), 0, 255));
    color.g = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.g + diffuse_illumination.g + specular_illumination.g), 0, 255));
    color.b = static_cast<models::Uint8>(math::Clamp(static_cast<float>(ambient_illumination.b + diffuse_illumination.b + specular_illumination.b), 0, 255));

    return color;
  }

  /**
   * @brief Calcula a iluminação de um objeto utilizando o modelo de iluminação de Phong
   *
   * @param light Luz ambiente da cena
   * @param omni Vetor de Lampa omnidirecionais
   * @param vertex Vértice da face e Normal médio do vértice
   * @param eye Posição do observador (câmera)
   * @param material Material do objeto
   *
   * @return models::Color Cor do vértice
   */
  models::Color PhongShading(const models::Light &light, const std::vector<models::Omni> &omni, const core::Vector3 &centroid, const std::pair<core::Vector3, core::Vector3> &vertex, const core::Vector3 &eye, const models::Material &material)
  {
    return PhongIllumination(light, omni, centroid, vertex.first, vertex.second, eye, material);
  }

} // namespace models