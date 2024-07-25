#include <math/pipeline.hpp>
#include <math/math.hpp>

namespace math
{

  /**
   * @brief Obtém a matriz de transformação de SRU para SRC.
   * @note SRU: Sistema de Referência do Universo
   * @note SRC: Sistema de Referência da Câmera
   *
   * A matriz de transformação obtida é dada por:
   *
   * | u.x  u.y  u.z  -u.vrp |
   * | v.x  v.y  v.z  -v.vrp |
   * | n.x  n.y  n.z  -n.vrp |
   * | 0    0    0    1      |
   *
   * @param vrp Vetor de posição da câmera
   * @param fp Ponto para onde a câmera está olhando
   * @return Matriz de transformação de SRU para SRC
   */
  core::Matrix sru_to_src(const core::Vector3 &vrp, const core::Vector3 fp)
  {
    // Define the n vector.
    core::Vector3 n = {vrp.x - fp.x, vrp.y - fp.y, vrp.z - fp.z};
    core::Vector3 n_normalized = math::Vector3Normalize(n);

    // Define the v vector.
    // TODO: Choose the up vector, depending on the camera orientation.
    core::Vector3 up_vec = {0, 1, 0};

    float y1 = math::Vector3DotProduct(up_vec, n_normalized);

    core::Vector3 _y1 = math::Vector3MultiplyValue(n_normalized, y1);

    core::Vector3 v = {up_vec.x - _y1.x, up_vec.y - _y1.y, up_vec.z - _y1.z};

    core::Vector3 v_normalized = math::Vector3Normalize(v);

    // Define the u vector.
    core::Vector3 u = math::Vector3CrossProduct(v_normalized, n_normalized);

    // Make the transformation matrix.
    core::Matrix result = core::Flota16ToMatrix({{u.x, u.y, u.z, -math::Vector3DotProduct(u, vrp),
                                                  v_normalized.x, v_normalized.y, v_normalized.z, -math::Vector3DotProduct(v_normalized, vrp),
                                                  n_normalized.x, n_normalized.y, n_normalized.z, -math::Vector3DotProduct(n_normalized, vrp),
                                                  0, 0, 0, 1}});

    return result;
  }

  /**
   * @brief Obtém a matriz de projeção.
   *
   * A matriz de projeção obtida é dada por:
   *
   * | 1  0  0  0 |
   * | 0  1  0  0 |
   * | 0  0  -z_vp/dp  z_vp*z_prp/dp |
   * | 0  0  -1/d  z_prp/dp |
   *
   * @param vrp Vetor de posição da câmera
   * @param p Vetor 3D que representa o ponto focal da câmera
   * @param d Distância do VRP ao plano ponto focal
   * @return Matriz de projeção
   */
  core::Matrix projection(const core::Vector3 &vrp, const core::Vector3 p, const float d)
  {
    // This is the definitions of the projection plane.
    core::Vector3 projection_plane = {
        vrp.x + (p.x - vrp.x) * (d / (vrp.z - p.z)),
        vrp.y + (p.y - vrp.y) * (d / (vrp.z - p.z)),
        vrp.z + (p.z - vrp.z) * (d / (vrp.z - p.z))};

    // The distance from the VRP to the projection plane.
    float dp = math::Vector3Distance(projection_plane, vrp);
    // The distance from the VRP to the focal point.
    float z_vp = -dp;
    // Z coordinate of the point, where the projection lines intersect the projection plane.
    // In this case, the z_prp is 0. Because this point coincides with the origin of the SRC (0, 0, 0).
    float z_prp = 0;

    core::Matrix result = core::Flota16ToMatrix({1, 0, 0, 0,
                                                 0, 1, 0, 0,
                                                 0, 0, (-z_vp) / dp, z_vp * (z_prp) / dp,
                                                 0, 0, -1 / d, z_prp / dp});

    return result;
  }

  core::Matrix src_to_srt(const core::Vector2 min_window, const core::Vector2 min_viewport, const core::Vector2 max_window, const core::Vector2 max_viewport, bool reflected = false)
  {
    float u_min = min_viewport.x;
    float u_max = max_viewport.x;
    float v_min = min_viewport.y;
    float v_max = max_viewport.y;

    float x_min = min_window.x;
    float x_max = max_window.x;
    float y_min = min_window.y;
    float y_max = max_window.y;

    core::Matrix result;

    if (reflected)
    {
      // | (u_max - u_min)/(x_max - x_min), 0, 0, -x_min *((u_max - u_min)/(x_max - x_min)) + u_min |\n
      // | 0, (v_min - v_max)/(y_max - y_min), 0, y_min * ((v_max - v_min)/(y_max - y_min)) + v_max |\n
      // | 0, 0, 1, 0 |\n
      // | 0, 0, 0, 1 |
      result = core::Flota16ToMatrix({(u_max - u_min) / (x_max - x_min), 0, 0, -x_min * ((u_max - u_min) / (x_max - x_min)) + u_min,
                                      0, (v_min - v_max) / (y_max - y_min), 0, y_min * ((v_max - v_min) / (y_max - y_min)) + v_max,
                                      0, 0, 1, 0,
                                      0, 0, 0, 1});
    }
    else
    {
      // | (u_max - u_min)/(x_max - x_min), 0, 0, -x_min * ((u_max - u_min)/(x_max - x_min)) + u_min |\n
      // | 0, (v_max - v_min)/(y_max - y_min), 0, -y_min * ((v_max - v_min)/(y_max - y_min)) + v_min) |\n
      // | 0, 0, 1, 0 |\n
      // | 0, 0, 0, 1 |
      result = core::Flota16ToMatrix({(u_max - u_min) / (x_max - x_min), 0, 0, -x_min * ((u_max - u_min) / (x_max - x_min)) + u_min,
                                      0, (v_max - v_min) / (y_max - y_min), 0, -y_min * ((v_max - v_min) / (y_max - y_min)) + v_min,
                                      0, 0, 1, 0,
                                      0, 0, 0, 1});
    }

    return result;
  }
} // namespace math