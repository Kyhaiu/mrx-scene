#include <models/light.hpp>

namespace models
{
  //-------------------------------------------------------------------------------------------------
  // Funções
  //-------------------------------------------------------------------------------------------------

  void FlatShading(const models::Light &light, const models::Omni &omni, const core::Vector3 &face_centroid, const core::Vector3 &eye, models::Color &color, const models::Material &material)
  {
    // Passo 1: Calcular a iluminação ambiente
    models::Color ambient_illumination;

    models::AmbientLight ambient = light.ambient;

    ambient_illumination.r = static_cast<models::Uint8>(math::Clamp(ambient.intensity.r * ambient.ka.r, 0, 255));
    ambient_illumination.g = static_cast<models::Uint8>(math::Clamp(ambient.intensity.g * ambient.ka.g, 0, 255));
    ambient_illumination.b = static_cast<models::Uint8>(math::Clamp(ambient.intensity.b * ambient.ka.b, 0, 255));

    // Passo 2: Calcular a iluminação difusa

    core::Vector3 centroid_normalized = math::Vector3Normalize(face_centroid);

    // Vetor da luz (direção da luz)
    core::Vector3 L = math::Vector3Normalize(math::Vector3Subtract(omni.position, face_centroid));

    float cos_theta = std::max(math::Vector3DotProduct(centroid_normalized, L), 0.0f);

    models::Color diffuse_illumination;
    models::ColorChannels kd = material.diffuse;

    diffuse_illumination.r = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.r * kd.r * cos_theta, 0, 255));
    diffuse_illumination.g = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.g * kd.g * cos_theta, 0, 255));
    diffuse_illumination.b = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.b * kd.b * cos_theta, 0, 255));

    // Passo 3: Calcular a iluminação especular

    // Vetor da reflexão da luz (R = (2N.L). N - L)
    core::Vector3 R = math::Vector3Subtract(math::Vector3MultiplyValue(centroid_normalized, 2 * cos_theta), L);
    core::Vector3 S = math::Vector3Normalize(math::Vector3Subtract(eye, centroid_normalized));

    core::Vector3 reflected_direction = math::Vector3Reflect(math::Vector3Negate(L), centroid_normalized);

    float cos_alpha = std::max(math::Vector3DotProduct(reflected_direction, S), 0.0f);

    models::Color specular_illumination;
    models::ColorChannels ks = material.specular;
    float n = material.shininess;

    specular_illumination.r = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.r * ks.r * pow(cos_alpha, n), 0, 255));
    specular_illumination.g = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.g * ks.g * pow(cos_alpha, n), 0, 255));
    specular_illumination.b = static_cast<models::Uint8>(math::Clamp(light.ambient.intensity.b * ks.b * pow(cos_alpha, n), 0, 255));

    // Passo 4: Calcular a cor final
    color.r = ambient_illumination.r + diffuse_illumination.r + specular_illumination.r;
    color.g = ambient_illumination.g + diffuse_illumination.g + specular_illumination.g;
    color.b = ambient_illumination.b + diffuse_illumination.b + specular_illumination.b;
  }
} // namespace models