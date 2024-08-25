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
    float dp = d;
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

  //-------------------------------------------------------------------------------------------------
  // Clipagem de Linhas
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Calcula o código de saída de um ponto em relação a uma janela de recorte.
   *
   * @param p Ponto a ser verificado
   * @param min Canto inferior esquerdo da janela de recorte
   * @param max Canto superior direito da janela de recorte
   * @return Código de saída do ponto
   *
   * @note O código de saída é uma combinação de bits que indicam a posição do ponto em relação à janela de recorte.
   * @note O código de saída é calculado da seguinte forma:
   * @note 0000: Ponto dentro da janela
   * @note 0001: Ponto à esquerda da janela
   * @note 0010: Ponto à direita da janela
   * @note 0100: Ponto abaixo da janela
   * @note 1000: Ponto acima da janela
   */
  int compute_outcode(core::Vector3 p, core::Vector2 min, core::Vector2 max)
  {
    int code = INSIDE;

    if (p.x < min.x) // to the left of rectangle
    {
      code |= LEFT;
    }
    else if (p.x > max.x) // to the right of rectangle
    {
      code |= RIGHT;
    }

    if (p.y < min.y) // below the rectangle
    {
      code |= BOTTOM;
    }
    else if (p.y > max.y) // above the rectangle
    {
      code |= TOP;
    }

    return code;
  }

  /**
   * @brief Recorta uma linha em relação a uma janela de recorte.
   *
   * @param p1 Ponto inicial da linha
   * @param p2 Ponto final da linha
   * @param min Canto inferior esquerdo da janela de recorte
   * @param max Canto superior direito da janela de recorte
   *
   * @return Par de pontos que representam a linha recortada
   *
   * @note A função recorta uma linha em relação a uma janela de recorte.
   * @note O algoritmo utilizado é o de Cohen-Sutherland.
   * @note Se a linha estiver inteiramente fora da janela, os pontos são modificados para (-1, -1).
   * @note A função retorna um par de pontos que representam a linha recortada.
   */
  std::pair<core::Vector3, core::Vector3> clip_line(core::Vector3 p1, core::Vector3 p2, core::Vector2 min, core::Vector2 max)
  {
    // Computa os códigos das extremidades da linha
    int P = compute_outcode(p1, min, max);
    int Q = compute_outcode(p2, min, max);

    while (true)
    {
      if (P == 0 && Q == 0) // Ambos os pontos estão dentro da janela
      {
        break;
      }
      else if (P & Q) // Ambos os pontos estão fora da janela
      {
        // Valores usados para indicar que a linha não está dentro da janela e não precisa ser desenhada
        p1.x = -1.0f;
        p1.y = -1.0f;
        p2.x = -1.0f;
        p2.y = -1.0f;
        break;
      }
      else
      {
        // Alguns dos segmento está dentro da janela
        int outcode = Q > P ? Q : P;
        float x, y;

        // Encontra o ponto de interseção
        // Usando a fórmula y = y1 + coeficiente de inclinação * (x - x1)
        // x = x1 + (1 / coeficiente de inclinação) * (y - y1)
        if (outcode & TOP) // O Ponto está acima da janela
        {
          x = p1.x + (p2.x - p1.x) * (max.y - p1.y) / (p2.y - p1.y);
          y = max.y;
        }
        else if (outcode & BOTTOM) // O Ponto está abaixo da janela
        {
          x = p1.x + (p2.x - p1.x) * (min.y - p1.y) / (p2.y - p1.y);
          y = min.y;
        }
        else if (outcode & RIGHT) // O Ponto está à direita da janela
        {
          y = p1.y + (p2.y - p1.y) * (max.x - p1.x) / (p2.x - p1.x);
          x = max.x;
        }
        else if (outcode & LEFT) // O Ponto está à esquerda da janela
        {
          y = p1.y + (p2.y - p1.y) * (min.x - p1.x) / (p2.x - p1.x);
          x = min.x;
        }

        // Move o ponto externo para a interseção
        if (outcode == P)
        {
          p1.x = x;
          p1.y = y;
          P = compute_outcode(p1, min, max);
        }
        else
        {
          p2.x = x;
          p2.y = y;
          Q = compute_outcode(p2, min, max);
        }
      }
    }

    return std::make_pair(p1, p2);
  }

  /**
   * @brief Algoritmo para interpolar os valores das coordenadas x e z de um polígono.
   *
   * @param vertexes Lista de vértices que compõem o polígono (coordenadas de tela)
   * @return std::vector<std::vector<core::Vector3>>
   */
  std::vector<std::vector<core::Vector3>> fill_polygon(std::vector<core::Vector3> vertexes)
  {
    // encontra o y_min e y_max
    int y_max = static_cast<int>(std::max_element(vertexes.begin(),
                                                  vertexes.end(),
                                                  [](const core::Vector3 &a, const core::Vector3 &b)
                                                  { return a.y < b.y; })
                                     ->y);
    int y_min = static_cast<int>(std::min_element(vertexes.begin(),
                                                  vertexes.end(),
                                                  [](const core::Vector3 &a, const core::Vector3 &b)
                                                  { return a.y < b.y; })
                                     ->y);

    int number_of_lines = y_max - y_min;

    // Vetor que contem as arestas do polígono
    std::vector<std::vector<core::Vector3>> edges;

    // O vetor de vertices é percorrido de 2 em 2
    // Pois cada par de vértices representa uma aresta
    for (int i = 0; i < vertexes.size(); i = i + 2)
    {
      edges.push_back({vertexes[i], vertexes[i + 1]});
    }

    // Cria um vetor de interseções
    std::vector<std::vector<core::Vector3>> intersections(number_of_lines - 1);

    // Calcula as interseções
    for (int y = y_min; y < y_max; y++)
    {
      for (auto edge : edges)
      {
        int x0 = static_cast<int>(edge[0].x);
        int y0 = static_cast<int>(edge[0].y);
        float z0 = edge[0].z;
        int x1 = static_cast<int>(edge[1].x);
        int y1 = static_cast<int>(edge[1].y);
        float z1 = edge[1].z;

        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        float dz = abs(z1 - z0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
          if (y0 == y)
          {
            float z = z0 + dz * (y - y0) / dy;
            core::Vector3 intersection = {static_cast<float>(x0), static_cast<float>(y), z};
            intersections[y - y_min].push_back(intersection);
          }

          if (x0 == x1 && y0 == y1)
            break;

          int e2 = 2 * err;
          if (e2 > -dy)
          {
            err = err - dy;
            x0 = x0 + sx;
          }
          if (e2 < dx)
          {
            err = err + dx;
            y0 = y0 + sy;
          }
        }
      }
    }

    return intersections;
  }

} // namespace math