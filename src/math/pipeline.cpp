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

    // Matriz I
    // | 1 0 0                                  0     |
    // | 0 1 0                                  0     |
    // | 0 0 (-z_min^2 + 2*z_min)/(-z_min + 1) -z_min |
    // | 0 0 (-z_min + 1)/z_min                 1     |
    float z_min = near / far;
    core::Matrix result = core::Flota16ToMatrix({1, 0, 0, 0,
                                                 0, 1, 0, 0,
                                                 0, 0, (-std::powf(z_min, 2) + 2 * z_min) / (-z_min + 1), -z_min,
                                                 0, 0, (-z_min + 1) / z_min, 1});

    // multiplicar as 3 primeira linhas da matriz I por 1/z_min.
    // Leva o tronco da piramide no prisma com dimensões 2*z_min em x, y e em z_min em z.
    // linha 1
    result.m0 *= 1 / z_min;
    result.m4 *= 1 / z_min;
    result.m8 *= 1 / z_min;
    result.m12 *= 1 / z_min;
    // linha 2
    result.m1 *= 1 / z_min;
    result.m5 *= 1 / z_min;
    result.m9 *= 1 / z_min;
    result.m13 *= 1 / z_min;
    // linha 3
    result.m2 *= 1 / z_min;
    result.m6 *= 1 / z_min;
    result.m10 *= 1 / z_min;
    result.m14 *= 1 / z_min;

    // multiplicar a matriz pelo escalar z_min
    // Faz a projeção perspectiva levando o VRP para o infinito.
    // A projeção passa a ser uma proj. paralela ortográfica ao ignorar a coordenada z.
    result = math::MatrixMultiplyValue(result, z_min);

    return result;
  }

  //-------------------------------------------------------------------------------------------------
  // Clipagem de Clipping (Clipagem de Linha)
  //-------------------------------------------------------------------------------------------------

  /**
   * @brief Calcula a coordenada y do ponto de interseção entre duas linhas.
   *
   * @param x1 Coordenada x do primeiro ponto da primeira linha.
   * @param y1 Coordenada y do primeiro ponto da primeira linha.
   * @param x2 Coordenada x do segundo ponto da primeira linha.
   * @param y2 Coordenada y do segundo ponto da primeira linha.
   * @param x3 Coordenada x do primeiro ponto da segunda linha.
   * @param y3 Coordenada y do primeiro ponto da segunda linha.
   * @param x4 Coordenada x do segundo ponto da segunda linha.
   * @param y4 Coordenada y do segundo ponto da segunda linha.
   *
   * @return int Coordenada y do ponto de interseção.
   *
   * @note A função utiliza a fórmula da interseção de duas linhas no plano 2D.
   * @note A interseção é calculada usando a regra de Cramer para resolver o sistema de equações lineares.
   * @note Se as linhas forem paralelas, o denominador será zero, e o comportamento é indefinido.
   */
  float y_intersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
  {
    float num = (x1 * y2 - y1 * x2) * (y3 - y4) -
                (y1 - y2) * (x3 * y4 - y3 * x4);
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    return floorf(num / den);
  }

  /**
   * @brief Calcula a coordenada x do ponto de interseção entre duas linhas.
   *
   * @param x1 Coordenada x do primeiro ponto da primeira linha.
   * @param y1 Coordenada y do primeiro ponto da primeira linha.
   * @param x2 Coordenada x do segundo ponto da primeira linha.
   * @param y2 Coordenada y do segundo ponto da primeira linha.
   * @param x3 Coordenada x do primeiro ponto da segunda linha.
   * @param y3 Coordenada y do primeiro ponto da segunda linha.
   * @param x4 Coordenada x do segundo ponto da segunda linha.
   * @param y4 Coordenada y do segundo ponto da segunda linha.
   *
   * @return int Coordenada x do ponto de interseção.
   *
   * @note A função utiliza a fórmula da interseção de duas linhas no plano 2D.
   * @note A interseção é calculada usando a regra de Cramer para resolver o sistema de equações lineares.
   * @note Se as linhas forem paralelas, o denominador será zero, e o comportamento é indefinido.
   */
  float x_intersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
  {
    float num = (x1 * y2 - y1 * x2) * (x3 - x4) -
                (x1 - x2) * (x3 * y4 - y3 * x4);
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    return floorf(num / den);
  }

  /**
   * @brief Realiza a clipagem de uma linha.
   *
   * @param polygon Polígono a ser clipado.
   * @param x1 Coordenada x do primeiro ponto da linha.
   * @param y1 Coordenada y do primeiro ponto da linha.
   * @param x2 Coordenada x do segundo ponto da linha.
   * @param y2 Coordenada y do segundo ponto da linha.
   *
   * @return std::vector<core::Vector3> Polígono clipado.
   *
   * @note A função realiza a clipagem de uma linha em relação a uma janela de visualização.
   * @note A função utiliza o algoritmo de Cohen-Sutherland.
   */
  std::vector<core::Vector3> clip_line(const std::vector<core::Vector3> &polygon, float x1, float y1, float x2, float y2)
  {
    std::vector<core::Vector3> clipped_polygon;

    for (int i = 0; i < polygon.size(); i++)
    {
      int k = (i + 1) % polygon.size();
      float ix = polygon[i].x, iy = polygon[i].y;
      float kx = polygon[k].x, ky = polygon[k].y;

      float i_pos = (x2 - x1) * (iy - y1) - (y2 - y1) * (ix - x1);

      float k_pos = (x2 - x1) * (ky - y1) - (y2 - y1) * (kx - x1);

      // Caso 1: Ambos os pontos estão dentro da janela
      if (i_pos < 0 && k_pos < 0)
      {
        // Apenas o segundo ponto é adicionado
        clipped_polygon.push_back(polygon[k]);
      }
      // Caso 2: Apenas o primeiro ponto está fora da janela
      else if (i_pos >= 0 && k_pos < 0)
      {
        // Adiciona o ponto de interseção e o segundo ponto
        clipped_polygon.push_back({x_intersection(x1, y1, x2, y2, ix, iy, kx, ky), y_intersection(x1, y1, x2, y2, ix, iy, kx, ky), polygon[i].z});
        clipped_polygon.push_back(polygon[k]);
      }
      // Caso 3: Apenas o segundo ponto está fora da janela
      else if (i_pos < 0 && k_pos >= 0)
      {
        // Adiciona o ponto de interseção com a janela
        clipped_polygon.push_back({x_intersection(x1, y1, x2, y2, ix, iy, kx, ky), y_intersection(x1, y1, x2, y2, ix, iy, kx, ky), polygon[i].z});
      }
      // Caso 4: Ambos os pontos estão fora da janela, nenhum ponto é adicionado
    }

    return clipped_polygon;
  }

  /**
   * @brief Realiza a clipagem de um polígono.
   *
   * @param polygon Polígono a ser clipado.
   * @param min Coordenadas mínimas da janela de visualização.
   * @param max Coordenadas máximas da janela de visualização.
   *
   * @return std::vector<core::Vector3> Polígono clipado.
   *
   * @note A função realiza a clipagem de um polígono em relação a uma janela de visualização.
   * @note A função utiliza o algoritmo de Sutherland-Hodgman.
   */
  std::vector<core::Vector3> sutherland_hodgman(const std::vector<core::Vector3> &polygon, const core::Vector2 &min, const core::Vector2 &max)
  {
    std::vector<core::Vector3> result = polygon;

    // Clip against the left edge
    result = clip_line(result, min.x, min.y, min.x, max.y);

    // Clip against the bottom edge
    result = clip_line(result, min.x, max.y, max.x, max.y);

    // Clip against the right edge
    result = clip_line(result, max.x, max.y, max.x, min.y);

    // Clip against the top edge
    result = clip_line(result, max.x, min.y, min.x, min.y);

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

      for (int j = 0; j < scanline.size(); j++)
      {
        int k = (j + 1) % scanline.size();
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

    scanlines.clear();
  }

  /**
   * @brief Preenche um polígono com sombreamento de Gourand
   *
   * @param vertexes Vertices da face do polígono
   * @param vertexes_normal Vetores normais dos vértices
   * @param global_light Luz global
   * @param omni_lights Luzes omni
   * @param eye Posição do observador
   * @param face_centroid Centroide da face
   * @param object_material Material do objeto
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   */
  void fill_polygon_gourand(const std::vector<std::pair<core::Vector3, core::Vector3>> &_vertexes, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, const core::Vector3 &eye, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    // Usando para associar cada vértice com sua cor calculada
    std::vector<std::pair<core::Vector3, models::Color>> vertexes;

    for (auto vertex : _vertexes)
    {
      // utiliza o vertice nas coordenadas SRU para calcular a cor
      core::Vector3 v = vertex.first;
      core::Vector3 n = vertex.second;
      models::Color color = models::GouraudShading(global_light, omni_lights, std::make_pair(v, n), eye, object_material);
      vertexes.push_back(std::make_pair(vertex.first, color));
    }

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
    for (int i = 0; i < vertexes.size(); i += 2)
    {
      core::Vector3 start = vertexes[i].first;
      core::Vector3 end = vertexes[i + 1].first;

      models::Color start_color = vertexes[i % vertexes.size()].second;
      models::Color end_color = vertexes[(i + 1) % vertexes.size()].second;

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

    for (int count = 0; count < _vertexes.size(); count += 2)
    {
      core::Vector3 start = _vertexes[count].first;
      core::Vector3 end = _vertexes[count + 1].first;

      core::Vector3 start_normal = _vertexes[count].second;
      core::Vector3 end_normal = _vertexes[count + 1].second;

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