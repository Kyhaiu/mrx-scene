#include <utils/utils.hpp>
#include <iostream>

namespace utils
{
  /**
   * @brief Desenha um vértice na janela
   *
   * @param v Referência para a janela onde o vértice será desenhado
   * @param vertex Vértice a ser desenhado
   *
   * @note O vértice é a classe Vertex do namespace core e não o tipo Vector3
   */
  void DrawVertex(ImDrawList *draw_list, const core::Vertex &vertex)
  {

    float x = vertex.getX(true);
    float y = vertex.getY(true);

    draw_list->AddCircle({x, y}, 2.0f,
                         IM_COL32(255, 255, 255, 255), 0);

    draw_list->AddText({x, y + 5}, IM_COL32(255, 255, 255, 255), vertex.getId().c_str());
  }

  /**
   * @brief Desenha uma linha na janela
   *
   * @param window Referência para a janela onde a linha será desenhada
   * @param vertexes Vetor de vértices que compõem a face
   * @param color Cor da linha
   */
  void DrawLine(ImDrawList *draw_list, const std::vector<core::Vector3> vertexes, const SDL_Color &color)
  {
    int vertex_length = vertexes.size();
    for (size_t i = 0; i < vertex_length - 1; i++)
    {
      // Se o vértice for válido
      if (vertexes[i].x != -1 && vertexes[i + 1].x != -1)
      {
        std::vector<core::Vector3> line = BresenhamLine(vertexes[i], vertexes[i + 1]);

        draw_list->AddLine({vertexes[i].x, vertexes[i].y},
                           {vertexes[i + 1].x, vertexes[i + 1].y},
                           GET_COLOR_UI32(color), 1.0f);
      }
    }
  }

  /**
   * @brief Desenha um pixel na janela
   *
   * @param x Coordenada x do pixel
   * @param y Coordenada y do pixel
   * @param z Coordenada z do pixel
   * @param color Cor do pixel
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param window_size Tamanho da janela
   *
   * @todo Refatorar para colocar o z_buffer e color_buffer como funções do pipeline
   */
  void setPixel(int x, int y, float z, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size)
  {
    // Check if the coordinates are within the bounds of the window
    if (x < 0 || x >= static_cast<int>(window_size.x) || y < 0 || y >= static_cast<int>(window_size.y))
      return;

    // Update the pixel only if the current pixel is closer (has a lower z value)
    if (z <= z_buffer[x][y])
    {
      z_buffer[x][y] = z;
      color_buffer[x][y] = color;
    }
  }

  /**
   * @brief Desenha um buffer na janela
   *
   * @param draw_list Referência para a janela onde o buffer será desenhado
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param window_size Tamanho da janela
   */
  void DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size)
  {
    int width = static_cast<int>(window_size.x);
    int height = static_cast<int>(window_size.y);

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        if (!CompareColors(color_buffer[x][y], TRANSPARENT_COLOR))
          draw_list->AddRectFilled({static_cast<float>(x), static_cast<float>(y)}, {static_cast<float>(x + 1), static_cast<float>(y + 1)}, GET_COLOR_UI32(color_buffer[x][y]));
      }
    }
  }

  /**
   * @brief Desenha um vértice no buffer
   *
   * @param draw_list Referência para a janela onde o buffer será desenhado
   * @param vertex Vértice a ser desenhado
   * @param color Cor do vértice
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param window_size Tamanho da janela
   */
  void DrawVertexBuffer(ImDrawList *draw_list, const core::Vertex &vertex, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size)
  {
    int x = static_cast<int>(vertex.getX(true));
    int y = static_cast<int>(vertex.getY(true));

    if (x < 0 || y < 0 || x >= window_size.x || y >= window_size.y)
      // Caso o vértice esteja fora da janela, não desenha
      return;

    for (int i = -2; i < 3; i++)
    {
      for (int j = -2; j < 3; j++)
      {
        setPixel(x + i, y + j, -9999, color, z_buffer, color_buffer, window_size);
      }
    }
  }

  /**
   * @brief Desenha uma linha no buffer
   *
   * @param draw_list Referência para a janela onde o buffer será desenhado
   * @param vertexes Vetor de vértices que compõem a linha
   * @param color Cor da linha
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   * @param window_size Tamanho da janela
   *
   * @note O Algoritmo implementado é o do Bresenham adaptado para interpolação de Z também.
   */
  void DrawLineBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> &vertexes, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size)
  {
    int vertex_length = vertexes.size();
    for (size_t i = 0; i < vertex_length - 1; i = i + 2)
    {
      // Se o vértice for válido
      if (vertexes[i].x != -1 && vertexes[i + 1].x != -1)
      {
        std::vector<core::Vector3> line = BresenhamLine(vertexes[i], vertexes[i + 1]);

        for (auto vertex : line)
        {
          setPixel(static_cast<int>(vertex.x), static_cast<int>(vertex.y), vertex.z, color, z_buffer, color_buffer, window_size);
        }
      }
    }
  }

  /**
   * @brief Desenha uma face no buffer
   *
   * @param draw_list Referência para a janela onde o buffer será desenhado
   * @param vertexes Vetor de vértices que compõem a face
   * @param color Cor da face
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   * @todo Arrumar bug de preenchimento
   */
  void DrawFaceBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> vertexes, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size)
  {
    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto vertex : vertexes)
    {
      if (vertex.y < y_min)
        y_min = static_cast<int>(vertex.y);
      if (vertex.y > y_max)
        y_max = static_cast<int>(vertex.y);
    }

    std::vector<std::vector<core::Vector3>> scanlines(y_max - y_min + 1);

    // Como a lista de vertices é composta pelo inicio e fim da aresta, o incremento é de 2
    for (int i = 0; i < vertexes.size(); i += 2)
    {
      core::Vector3 start = vertexes[i];
      core::Vector3 end = vertexes[i + 1];

      if (start.y == end.y)
        continue;

      if (start.y > end.y)
      {
        std::swap(start, end);
      }

      for (int y = static_cast<int>(start.y); y <= static_cast<int>(end.y); y++)
      {
        float t = (y - start.y) / (end.y - start.y);
        float x = start.x + t * (end.x - start.x);
        float z = start.z + t * (end.z - start.z);
        scanlines[y - y_min].push_back({x, static_cast<float>(y), z});
      }
    }

    // problema aqui
    for (auto scanline : scanlines)
    {
      std::sort(scanline.begin(), scanline.end(), [](core::Vector3 a, core::Vector3 b)
                { return a.x < b.x; });

      for (int i = 0; i < scanline.size(); i = i + 2)
      {

        core::Vector3 start = scanline[i];
        core::Vector3 end = scanline[i + 1];

        for (int x = static_cast<int>(start.x); x <= static_cast<int>(end.x); x++)
        {
          setPixel(x, static_cast<int>(start.y), start.z + (end.z - start.z) * (x - start.x) / (end.x - start.x), color, z_buffer, color_buffer, window_size);
        }
      }
    }
  }

  /**
   * @brief Algoritmo de Bresenham para desenhar uma linha
   *
   * @param start Ponto de início da linha
   * @param end Ponto de fim da linha
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
} // namespace utils