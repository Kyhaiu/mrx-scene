#include <math/pipeline.hpp>
#include <math/math.hpp>

#include <iostream>

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
  core::Matrix pipeline_adair::sru_to_src(const core::Vector3 &vrp, const core::Vector3 fp)
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
  core::Matrix pipeline_adair::projection(const core::Vector3 &vrp, const core::Vector3 p, const float d)
  {

    // This is the definitions of the projection plane.
    core::Vector3 projection_plane = {
        vrp.x + (p.x - vrp.x) * (d / (vrp.z - p.z)),
        vrp.y + (p.y - vrp.y) * (d / (vrp.z - p.z)),
        vrp.z + (p.z - vrp.z) * (d / (vrp.z - p.z))};

    // The distance from the VRP to the projection plane.
    float dp = d;
    // The distance from the VRP to the focal point.
    float z_vp = -dp;
    // Z coordinate of the point, where the projection lines intersect the projection plane.
    // In this case, the z_prp is 0. Because this point coincides with the origin of the SRC (0, 0, 0).
    float z_prp = 0;

    // core::Matrix result = core::Flota16ToMatrix({1, 0, 0, 0,
    //                                              0, 1, 0, 0,
    //                                              0, 0, 0, 0,
    //                                              0, 0, 0, 1});
    core::Matrix result = core::Flota16ToMatrix({1, 0, 0, 0,
                                                 0, 1, 0, 0,
                                                 0, 0, (-z_vp) / dp, z_vp * (z_prp) / dp,
                                                 0, 0, -1 / d, z_prp / dp});

    return result;
  }

  /**
   * @brief Obtém a matriz de transformação de SRC para SRT.
   *
   * A matriz de transformação obtida é dada por:
   *
   * | (u_max - u_min)/(x_max - x_min)  0  0  -x_min *((u_max - u_min)/(x_max - x_min)) + u_min |
   * | 0  (v_min - v_max)/(y_max - y_min)  0  y_min * ((v_max - v_min)/(y_max - y_min)) + v_max |
   * | 0  0  1  0 |
   * | 0  0  0  1 |
   *
   * @param min_window Vetor 2D que representa o canto inferior esquerdo da janela
   * @param min_viewport Vetor 2D que representa o canto inferior esquerdo da viewport
   * @param max_window Vetor 2D que representa o canto superior direito da janela
   * @param max_viewport Vetor 2D que representa o canto superior direito da viewport
   * @param reflected Flag que indica se a transformação é refletida
   * @return Matriz de transformação de SRC para SRT
   */
  core::Matrix pipeline_adair::src_to_srt(const core::Vector2 min_window, const core::Vector2 min_viewport, const core::Vector2 max_window, const core::Vector2 max_viewport, bool reflected = false)
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

  /**
   * @brief Obtém a matriz de transformação de recorte.
   *
   * @param d Distância do VRP ao plano de projeção
   * @param far Plano de projeção distante
   * @param center_window Centro da janela
   * @param size_window Tamanho da janela
   *
   * @note O tamanho da janela é metade da largura e metade da altura
   *
   * @return core::Matrix
   */
  core::Matrix pipeline_smith::clipping_transformation(const float d, const float far, const core::Vector2 center_window, const core::Vector2 size_window)
  {
    // cu = center window x
    // cv = center window y
    // su = size window x
    // sv = size window y
    // | d/(su*far) 0          -(cu/d*far) 0 |
    // | 0          d/(sv*far) -(cv/d*far) 0 |
    // | 0          0          1/far       0 |
    // | 0          0          0           1 |

    float cu = center_window.x;
    float cv = center_window.y;
    float su = size_window.x / 2;
    float sv = size_window.y / 2;

    core::Matrix result = core::Flota16ToMatrix({d / (su * far), 0, -(cu / d * far), 0,
                                                 0, d / (sv * far), -(cv / d * far), 0,
                                                 0, 0, 1 / far, 0,
                                                 0, 0, 0, 1});

    return result;
  }

  /**
   * @brief Obtém a matrix de transformação perspectiva
   *
   * @note A transformação de perspectiva opera diretamente no espaço 3D e transforma o frustum em um paralelepípedo canônico com lados paralelos e comprimento unitário.
   *
   * @param near Plano de projeção próximo
   * @param far Plano de projeção distante
   * @return core::Matrix
   */
  core::Matrix pipeline_smith::perspective_transformation(const float near, const float far)
  {
    // Obs.: A transformação de perspectiva é feita através das seguintes operações:
    // Translada z_min para a origem => F
    // Escala o volume canônico em z para que o plano traseiro (far) coincida com z = 1 => G
    // Realiza a transformação de perspectiva => H
    // A matriz resultante I é igual à H * G * F

    float z_min = near / far;

    core::Matrix F = math::MatrixTranslate({0.0f, 0.0f, -z_min});

    core::Matrix G = math::MatrixScale({1.0f, 1.0f, 1.0f / (1 - z_min)});

    core::Matrix H = core::Flota16ToMatrix({1, 0, 0, 0,
                                            0, 1, 0, 0,
                                            0, 0, 1, 0,
                                            0, 0, (1 - z_min) / z_min, 0});

    core::Matrix result = math::MatrixMultiply(H, G);
    result = math::MatrixMultiply(result, F);

    float inv_z_min = 1 / z_min;

    // multiplicar as 3 primeira linhas da matriz I por 1/z_min.
    // Leva o tronco da piramide no prisma com dimensões 2*z_min em x, y e em z_min em z.
    // linha 1
    result.m0 *= inv_z_min;
    result.m4 *= inv_z_min;
    result.m8 *= inv_z_min;
    result.m12 *= inv_z_min;
    // linha 2
    result.m1 *= inv_z_min;
    result.m5 *= inv_z_min;
    result.m9 *= inv_z_min;
    result.m13 *= inv_z_min;
    // linha 3
    result.m2 *= inv_z_min;
    result.m6 *= inv_z_min;
    result.m10 *= inv_z_min;
    result.m14 *= inv_z_min;

    // multiplicar a matriz pelo escalar z_min
    // Faz a projeção perspectiva levando o VRP para o infinito.
    // A projeção passa a ser uma proj. paralela ortográfica ao ignorar a coordenada z.
    result = math::MatrixMultiplyValue(result, z_min);

    return result;
  }

  core::Matrix pipeline_smith::src_to_srt(const core::Vector2 min_viewport, const core::Vector2 max_viewport, const float near, const float far)
  {

    core::Matrix K = core::Flota16ToMatrix({0.5f, 0.0f, 0.0f, 0.5f,
                                            0.0f, 0.5f, 0.0f, 0.5f,
                                            0.0f, 0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f});
    float dx = max_viewport.x - min_viewport.x;
    float dy = max_viewport.y - min_viewport.y;
    float dz = far - near;

    core::Matrix L = core::Flota16ToMatrix({dx, 0, 0, min_viewport.x,
                                            0, dy, 0, min_viewport.y,
                                            0, 0, dz, near,
                                            0, 0, 0, 1});

    core::Matrix M = K;

    core::Matrix result = math::MatrixMultiply(M, L);
    result = math::MatrixMultiply(result, K);

    return result;
  }

  //-------------------------------------------------------------------------------------------------
  // Clipagem de Clipping (Clipagem de Linha)
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Verifica se um ponto está antes ou depois de uma borda de uma janela de recorte.
   *
   * @param p Ponto a ser verificado
   * @param min Canto inferior esquerdo da janela de recorte
   * @param max Canto superior direito da janela de recorte
   * @param edge Borda da janela de recorte a ser verificada
   *
   * @return true Se o ponto está dentro da janela de recorte;
   * @return false Se o ponto está fora da janela de recorte
   */
  bool is_inside(core::Vector3 p, core::Vector2 min, core::Vector2 max, unsigned int edge)
  {
    switch (edge)
    {
    case LEFT:
      return p.x >= min.x; // Limite da borda da esquerda da janela de recorte é x_min
    case RIGHT:
      return p.x <= max.x; // Limite da borda da direita da janela de recorte é x_max
    case BOTTOM:
      return p.y >= min.y; // Limite da borda inferior da janela de recorte é y_min
    case TOP:
      return p.y <= max.y; // Limite da borda superior da janela de recorte é y_max
    default:
      return false; // Caso não seja nenhuma das bordas
    }
  }

  /**
   * @brief Calcula o ponto de interseção de uma linha com uma janela de recorte.
   *
   * @param p1 Ponto inicial da linha
   * @param p2 Ponto final da linha
   * @param min Canto inferior esquerdo da janela de recorte
   * @param max Canto superior direito da janela de recorte
   * @param edge Borda da janela de recorte
   * @return core::Vector3
   */
  core::Vector3 compute_intersection(core::Vector3 p1, core::Vector3 p2, core::Vector2 min, core::Vector2 max, unsigned int edge)
  {
    float u = 0.0f;
    core::Vector3 intersection;

    if (edge == LEFT)
    {
      u = (min.x - p1.x) / (p2.x - p1.x);
      intersection.x = min.x;
      intersection.y = p1.y + u * (p2.y - p1.y);
      intersection.z = p1.z + u * (p2.z - p1.z);
    }
    else if (edge == RIGHT)
    {
      u = (max.x - p1.x) / (p2.x - p1.x);
      intersection.x = max.x;
      intersection.y = p1.y + u * (p2.y - p1.y);
      intersection.z = p1.z + u * (p2.z - p1.z);
    }
    else if (edge == BOTTOM)
    {
      u = (min.y - p1.y) / (p2.y - p1.y);
      intersection.x = p1.x + u * (p2.x - p1.x);
      intersection.y = min.y;
      intersection.z = p1.z + u * (p2.z - p1.z);
    }
    else if (edge == TOP)
    {
      u = (max.y - p1.y) / (p2.y - p1.y);
      intersection.x = p1.x + u * (p2.x - p1.x);
      intersection.y = max.y;
      intersection.z = p1.z + u * (p2.z - p1.z);
    }

    return intersection;
  }

  std::pair<core::Vector3, core::Vector3> compute_intersection(std::pair<core::Vector3, core::Vector3> p1, std::pair<core::Vector3, core::Vector3> p2, core::Vector2 min, core::Vector2 max, unsigned int edge)
  {
    float u = 0.0f;
    std::pair<core::Vector3, core::Vector3> intersection;

    if (edge == LEFT)
    {
      u = (min.x - p1.first.x) / (p2.first.x - p1.first.x);

      intersection.first.x = min.x;
      intersection.first.y = p1.first.y + u * (p2.first.y - p1.first.y);
      intersection.first.z = p1.first.z + u * (p2.first.z - p1.first.z);

      intersection.second.x = math::Lerp(p1.second.x, p2.second.x, u);
      intersection.second.y = math::Lerp(p1.second.y, p2.second.y, u);
      intersection.second.z = math::Lerp(p1.second.z, p2.second.z, u);
    }
    else if (edge == RIGHT)
    {
      u = (max.x - p1.first.x) / (p2.first.x - p1.first.x);

      intersection.first.x = max.x;
      intersection.first.y = p1.first.y + u * (p2.first.y - p1.first.y);
      intersection.first.z = p1.first.z + u * (p2.first.z - p1.first.z);

      intersection.second.x = math::Lerp(p1.second.x, p2.second.x, u);
      intersection.second.y = math::Lerp(p1.second.y, p2.second.y, u);
      intersection.second.z = math::Lerp(p1.second.z, p2.second.z, u);
    }
    else if (edge == BOTTOM)
    {
      u = (min.y - p1.first.y) / (p2.first.y - p1.first.y);

      intersection.first.x = p1.first.x + u * (p2.first.x - p1.first.x);
      intersection.first.y = min.y;
      intersection.first.z = p1.first.z + u * (p2.first.z - p1.first.z);

      intersection.second.x = math::Lerp(p1.second.x, p2.second.x, u);
      intersection.second.y = math::Lerp(p1.second.y, p2.second.y, u);
      intersection.second.z = math::Lerp(p1.second.z, p2.second.z, u);
    }
    else if (edge == TOP)
    {
      u = (max.y - p1.first.y) / (p2.first.y - p1.first.y);

      intersection.first.x = p1.first.x + u * (p2.first.x - p1.first.x);
      intersection.first.y = max.y;
      intersection.first.z = p1.first.z + u * (p2.first.z - p1.first.z);

      intersection.second.x = math::Lerp(p1.second.x, p2.second.x, u);
      intersection.second.y = math::Lerp(p1.second.y, p2.second.y, u);
      intersection.second.z = math::Lerp(p1.second.z, p2.second.z, u);
    }

    return intersection;
  }

  /**
   * @brief Clipa um polígono 2D
   *
   * @param polygon Lista de vértices do polígono percorridos no sentido anti-horário
   * @param min Limite inferior esquerdo da janela de recorte
   * @param max Limite superior direito da janela de recorte
   * @return std::vector<core::Vector3> Lista de vértices do polígono clipado
   *
   * @note O algoritmo de Sutherland-Hodgman é utilizado
   */
  std::vector<core::Vector3> clip2D_polygon(const std::vector<core::Vector3> &polygon, const core::Vector2 &min, const core::Vector2 &max)
  {
    std::vector<core::Vector3> result = polygon;

    const std::vector<unsigned int> edges = {LEFT, RIGHT, BOTTOM, TOP};

    // A cada iteração, uma nova lista de vertices é gerada
    for (auto edge : edges)
    {
      if (result.empty())
        break;

      std::vector<core::Vector3> input = result;
      result.clear();

      for (int i = 0; i < input.size(); i++)
      {
        int k = (i + 1) % input.size();
        core::Vector3 p1 = input[i];
        core::Vector3 p2 = input[k];

        // Testa se os pontos estão dentro da janela de recorte
        bool p1_inside = is_inside(p1, min, max, edge);
        bool p2_inside = is_inside(p2, min, max, edge);

        // Ambos os pontos estão dentro da janela, então adiciona o ponto final
        if (p1_inside && p2_inside)
          result.push_back(p2);
        // Nenhum dos pontos está dentro da janela, então não adiciona nenhum ponto
        else if (!p1_inside && !p2_inside)
          continue;
        // Não é possível aceitar ou rejeitar trivialmente a linha
        else
        {
          // Calcula o ponto de interseção
          core::Vector3 intersection = compute_intersection(p1, p2, min, max, edge);

          // Somente o primeiro ponto está fora da janela, então adiciona o ponto de interseção e o ponto final
          if (!p1_inside && p2_inside)
          {
            result.push_back(intersection);
            result.push_back(p2);
          }
          // Somente o segundo ponto está fora da janela, então adiciona o ponto de interseção
          else if (p1_inside && !p2_inside)
            result.push_back(intersection);
        }
      }
    }
    return result;
  }

  std::vector<std::pair<core::Vector3, core::Vector3>> clip2D_polygon(const std::vector<std::pair<core::Vector3, core::Vector3>> &polygon, const core::Vector2 &min, const core::Vector2 &max)
  {
    std::vector<std::pair<core::Vector3, core::Vector3>> result = polygon;

    const std::vector<unsigned int> edges = {LEFT, RIGHT, BOTTOM, TOP};

    // A cada iteração, uma nova lista de vertices é gerada
    for (auto edge : edges)
    {
      if (result.empty())
        break;

      std::vector<std::pair<core::Vector3, core::Vector3>> input = result;
      result.clear();

      for (int i = 0; i < input.size(); i++)
      {
        int k = (i + 1) % input.size();
        std::pair<core::Vector3, core::Vector3> p1 = input[i];
        std::pair<core::Vector3, core::Vector3> p2 = input[k];

        // Testa se os pontos estão dentro da janela de recorte
        bool p1_inside = is_inside(p1.first, min, max, edge);
        bool p2_inside = is_inside(p2.first, min, max, edge);

        // Ambos os pontos estão dentro da janela, então adiciona o ponto final
        if (p1_inside && p2_inside)
          result.push_back(p2);
        // Nenhum dos pontos está dentro da janela, então não adiciona nenhum ponto
        else if (!p1_inside && !p2_inside)
          continue;
        // Não é possível aceitar ou rejeitar trivialmente a linha
        else
        {
          // Calcula o ponto de interseção
          std::pair<core::Vector3, core::Vector3> intersection = compute_intersection(p1, p2, min, max, edge);

          // Somente o primeiro ponto está fora da janela, então adiciona o ponto de interseção e o ponto final
          if (!p1_inside && p2_inside)
          {
            result.push_back(intersection);
            result.push_back(p2);
          }
          // Somente o segundo ponto está fora da janela, então adiciona o ponto de interseção
          else if (p1_inside && !p2_inside)
            result.push_back(intersection);
        }
      }
    }
    return result;
  }

  //-------------------------------------------------------------------------------------------------
  // Funções de Preenchimento de Polígonos e Desenho de Linhas
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Algoritmo de Bresenham para desenhar uma linha
   *
   * @param start Ponto de início da linha
   * @param end Ponto de fim da linha
   *
   * @todo verificar a interpolação de Z
   *
   * @return Vetor de vértices que compõem a linha
   */
  std::vector<core::Vector3> BresenhamLine(core::Vector3 start, core::Vector3 end)
  {
    std::vector<core::Vector3> line;
    // Se o vértice for válido
    if (start.x != -1 && end.x != -1)
    {
      int x0 = static_cast<int>(start.x);
      int y0 = static_cast<int>(start.y);
      int x1 = static_cast<int>(end.x);
      int y1 = static_cast<int>(end.y);

      float z0 = start.z;
      float z1 = end.z;

      int dx = abs(x1 - x0);
      int dy = abs(y1 - y0);
      int sx = (x0 < x1) ? 1 : -1;
      int sy = (y0 < y1) ? 1 : -1;
      int err = dx - dy;

      float dz = (z1 - z0) / std::max(dx, dy); // Step size for Z interpolation

      while (true)
      {

        line.push_back({static_cast<float>(x0), static_cast<float>(y0), z0});

        if (x0 == x1 && y0 == y1)
          break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
          err -= dy;
          x0 += sx;
          z0 += dz; // Interpolate Z
        }
        if (e2 < dx)
        {
          err += dx;
          y0 += sy;
          z0 += dz; // Interpolate Z
        }
      }
    }

    return line;
  }

  /**
   * @brief Preenche um polígono com sombreamento flat
   *
   * @param vertexes Vertices da face do polígono
   * @param color Cor do polígono
   * @param global_light Luz global
   * @param omni_lights Luzes omni
   * @param eye Posição do observador
   * @param face_centroid Centroide da face
   * @param face_normal Vetor normal da face
   * @param object_material Material do objeto
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param max_window_size Tamanho máximo da janela
   *
   */
  void fill_polygon_flat_shading(const std::vector<core::Vector3> &vertexes, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, const core::Vector3 &eye, const core::Vector3 &face_centroid, const core::Vector3 &face_normal, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, core::Vector2 max_window_size)
  {
    models::Color color = models::FlatShading(global_light, omni_lights, face_centroid, face_normal, eye, object_material);

    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto vertex : vertexes)
    {
      float y = vertex.y;
      if (y < y_min)
        y_min = static_cast<int>(y);
      if (y > y_max)
        y_max = static_cast<int>(y);
    }

    std::vector<std::vector<core::Vector3>> scanlines(y_max - y_min);

    // Como a lista de vertices é composta pelo inicio e fim da aresta, o incremento é de 2
    for (int i = 0; i < vertexes.size(); i++)
    {
      int k = (i + 1) % vertexes.size();
      core::Vector3 start = vertexes[i];
      core::Vector3 end = vertexes[k];

      if (start.y == end.y)
        continue;

      if (start.y > end.y)
      {
        std::swap(start, end);
      }

      float m_inv = (end.x - start.x) / (end.y - start.y);
      float mz = (end.z - start.z) / (end.y - start.y);

      float x = start.x;
      float z = start.z;

      for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
      {

        scanlines[y - y_min].push_back({x, static_cast<float>(y), z});

        x += m_inv;
        z += mz;
      }
    }

    for (auto scanline : scanlines)
    {
      std::sort(scanline.begin(), scanline.end(), [](core::Vector3 a, core::Vector3 b)
                { return a.x < b.x; });

      for (int j = 0; j < scanline.size(); j += 2)
      {
        int k = (j + 1);
        core::Vector3 start = scanline[j];
        core::Vector3 end = scanline[k];

        float mz = (end.z - start.z) / (end.x - start.x);
        float z = start.z + (start.x - ceilf(start.x)) * mz;

        for (float x = ceilf(start.x); x <= floorf(end.x); x++)
        {

          math::z_buffer(x, start.y, z, color, z_buffer, color_buffer);
          z += mz;
        }
      }
    }
  }

  /**
   * @brief Preenche um polígono com sombreamento de Gourand
   *
   * @param vertexes Vertices da face do polígono
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   */
  void fill_polygon_gourand(const std::vector<std::pair<core::Vector3, models::Color>> &_vertexes, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    // Usando para associar cada vértice com sua cor calculada
    std::vector<std::pair<core::Vector3, models::Color>> vertexes = _vertexes;

    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto vertex : vertexes)
    {
      float y = vertex.first.y;
      if (y < y_min)
        y_min = static_cast<int>(y);
      if (y > y_max)
        y_max = static_cast<int>(y);
    }

    // Vetor de scanlines
    // 1º parâmetro do par: vetor de coordenadas SRT (coordenadas de tela)
    // 2º parâmetro do par: cor do pixel
    std::vector<std::vector<std::pair<core::Vector3, models::Color>>> scanlines(y_max - y_min);

    // Como a lista de vertices é composta pelo inicio e fim da aresta, o incremento é de 2
    for (int i = 0; i < vertexes.size(); i++)
    {
      int k = (i + 1) % vertexes.size();

      core::Vector3 start = vertexes[i].first;
      core::Vector3 end = vertexes[k].first;

      models::Color start_color = vertexes[i].second;
      models::Color end_color = vertexes[k].second;

      // Se a linha for horizontal, não faz nada
      if (start.y == end.y)
        continue;

      if (start.y > end.y)
      {
        std::swap(start, end);
        std::swap(start_color, end_color);
      }

      float dy = end.y - start.y;

      float m_inv = (end.x - start.x) / dy;
      float dz = (end.z - start.z) / dy;

      float dr = (end_color.r - start_color.r) / dy;
      float dg = (end_color.g - start_color.g) / dy;
      float db = (end_color.b - start_color.b) / dy;

      float x = start.x;
      float z = start.z;

      float r = start_color.r;
      float g = start_color.g;
      float b = start_color.b;

      for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
      {

        scanlines[y - y_min].push_back(std::make_pair<core::Vector3, models::Color>({x, static_cast<float>(y), z}, {models::ChannelsToColor({r, g, b})}));
        x += m_inv;
        z += dz;

        r += dr;
        g += dg;
        b += db;
      }
    }

    for (int i = 0; i < scanlines.size(); i++)
    {
      std::sort(scanlines[i].begin(), scanlines[i].end(),
                [](std::pair<core::Vector3, models::Color> a, std::pair<core::Vector3, models::Color> b)
                { return a.first.x < b.first.x; });

      for (int j = 0; j < scanlines[i].size(); j = j + 2)
      {

        core::Vector3 start = scanlines[i][j].first;
        core::Vector3 end = scanlines[i][j + 1].first;

        models::Color start_color = scanlines[i][j].second;
        models::Color end_color = scanlines[i][j + 1].second;

        float dx = end.x - start.x;

        float dz = (end.z - start.z) / dx;
        float z = start.z + (start.x - ceilf(start.x)) * dz;

        float dr = (end_color.r - start_color.r) / dx;
        float dg = (end_color.g - start_color.g) / dx;
        float db = (end_color.b - start_color.b) / dx;

        float r = start_color.r + (start.x - ceilf(start.x)) * dr;
        float g = start_color.g + (start.x - ceilf(start.x)) * dg;
        float b = start_color.b + (start.x - ceilf(start.x)) * db;

        for (float x = ceilf(start.x); x <= floorf(end.x); x++)
        {
          models::Color current_color = models::ChannelsToColor({r, g, b});

          math::z_buffer(x, start.y, z, current_color, z_buffer, color_buffer);
          z += dz;
          r += dr;
          g += dg;
          b += db;
        }
      }
    }
  }

  /**
   * @brief Preenche um polígono com sombreamento de Phong
   *
   * @param _vertexes Lista de vertices e normais dos vertices
   * @param global_light Luz ambiente global
   * @param omni_lights Lista de luzes omnidirecionais
   * @param eye Posição do observador
   * @param object_material Material do objeto
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   */
  void fill_polygon_phong(const std::vector<std::pair<core::Vector3, core::Vector3>> &_vertexes, const core::Vector3 &centroid, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, const core::Vector3 &eye, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto vertex : _vertexes)
    {
      float y = vertex.first.y;
      if (y < y_min)
        y_min = static_cast<int>(y);
      if (y > y_max)
        y_max = static_cast<int>(y);
    }

    // Vetor de scanlines
    // 1º parâmetro do par: vetor de coordenadas SRT (coordenadas de tela)
    // 2º parâmetro do par: vetor normal do pixel (interpolado)
    std::vector<std::vector<std::tuple<core::Vector3, core::Vector3>>> scanlines(y_max - y_min);

    for (int l = 0; l < _vertexes.size(); l++)
    {
      int m = (l + 1) % _vertexes.size();
      core::Vector3 start = _vertexes[l].first;
      core::Vector3 end = _vertexes[m].first;

      core::Vector3 start_normal = _vertexes[l].second;
      core::Vector3 end_normal = _vertexes[m].second;

      // Se a linha for horizontal, não faz nada
      if (start.y == end.y)
        continue;

      if (start.y > end.y)
      {
        std::swap(start, end);
        std::swap(start_normal, end_normal);
      }

      float dy = end.y - start.y;
      float dx = end.x - start.x;
      float dz = end.z - start.z;
      // i,j,k se referem ao vetor normal do pixel (x, y, z)
      float dn_i = end_normal.x - start_normal.x;
      float dn_j = end_normal.y - start_normal.y;
      float dn_k = end_normal.z - start_normal.z;

      // taxa de variação do x em relação ao y
      float t_x = dx / dy;
      // taxa de variação do z em relação ao y
      float t_z = dz / dy;
      // taxa de variação do i em relação ao y
      float t_i = dn_i / dy;
      // taxa de variação do j em relação ao y
      float t_j = dn_j / dy;
      // taxa de variação do k em relação ao y
      float t_k = dn_k / dy;

      float x = start.x;
      float z = start.z;

      float i = start_normal.x;
      float j = start_normal.y;
      float k = start_normal.z;

      for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
      {
        scanlines[y - y_min].push_back(std::make_tuple<core::Vector3, core::Vector3>({x, static_cast<float>(y), z}, {i, j, k}));
        // incrementa com as taxas de variação (interpolação linear)
        x += t_x;
        z += t_z;
        i += t_i;
        j += t_j;
        k += t_k;
      }
    }

    for (int row = 0; row < scanlines.size(); row++)
    {
      std::sort(scanlines[row].begin(), scanlines[row].end(),
                [](std::tuple<core::Vector3, core::Vector3> a, std::tuple<core::Vector3, core::Vector3> b)
                { return std::get<0>(a).x < std::get<0>(b).x; });

      for (int col = 0; col < scanlines[row].size(); col += 2)
      {
        core::Vector3 start = std::get<0>(scanlines[row][col]);
        core::Vector3 end = std::get<0>(scanlines[row][col + 1]);

        core::Vector3 start_normal = std::get<1>(scanlines[row][col]);
        core::Vector3 end_normal = std::get<1>(scanlines[row][col + 1]);

        float dx = end.x - start.x;
        float dz = (end.z - start.z) / dx;
        float z = start.z + (start.x - ceilf(start.x)) * dz;

        float dn_i = (end_normal.x - start_normal.x) / dx;
        float dn_j = (end_normal.y - start_normal.y) / dx;
        float dn_k = (end_normal.z - start_normal.z) / dx;

        float i = start_normal.x;
        float j = start_normal.y;
        float k = start_normal.z;

        for (float x = ceilf(start.x); x <= floorf(end.x); x++)
        {
          core::Vector3 v = {x, start.y, z};
          core::Vector3 n = {i, j, k};

          models::Color color = models::PhongShading(global_light, omni_lights, centroid, std::make_pair(v, n), eye, object_material);
          math::z_buffer(x, start.y, z, color, z_buffer, color_buffer);
          z += dz;
          i += dn_i;
          j += dn_j;
          k += dn_k;
        }
      }
    }
  }

  /**
   * @brief Atualiza o buffer de profundidade
   *
   * @param z Profundidade do pixel
   * @param color Cor do pixel
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param window_size Tamanho da janela
   */
  void z_buffer(const float x, const float y, const float z, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    // Arredondamento para o pixel mais próximo
    int x_int = static_cast<int>(x);
    int y_int = static_cast<int>(y);

    if (x_int < 0 || x_int >= z_buffer.size() || y_int < 0 || y_int >= z_buffer[0].size())
      return;

    // Se o pixel atual estiver mais distante que o pixel já desenhado, não atualiza os buffers
    if (z_buffer[x_int][y_int] < z)
      return;

    z_buffer[x_int][y_int] = z;
    color_buffer[x_int][y_int] = color;
  }

} // namespace math