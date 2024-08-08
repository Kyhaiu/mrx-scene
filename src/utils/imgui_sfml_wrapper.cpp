#include <utils/imgui_sfml_wrapper.hpp>
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
    sf::CircleShape circle(2);
    circle.setFillColor(sf::Color::White);
    circle.setPosition(vertex.getX(true), vertex.getY(true));

    draw_list->AddCircle({circle.getPosition().x, circle.getPosition().y}, 2.0f,
                         IM_COL32(255, 255, 255, 255), 0);

    draw_list->AddText({circle.getPosition().x, circle.getPosition().y + 5}, IM_COL32(255, 255, 255, 255), vertex.getId().c_str());
  }

  /**
   * @brief Desenha uma linha na janela
   *
   * @param window Referência para a janela onde a linha será desenhada
   * @param vertexes Vetor de vértices que compõem a face
   * @param color Cor da linha
   */
  void DrawLine(ImDrawList *draw_list, const std::vector<core::Vector3> vertexes, const sf::Color &color)
  {
    int vertex_length = vertexes.size();
    for (size_t i = 0; i < vertex_length - 1; i++)
    {
      // Se o vértice for válido
      if (vertexes[i].x != -1 && vertexes[i + 1].x != -1)
      {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(vertexes[i].x, vertexes[i].y)),
            // Se for o último vértice, fecha a linha com o primeiro
            sf::Vertex(sf::Vector2f(vertexes[i + 1].x,
                                    vertexes[i + 1].y))};

        draw_list->AddLine({line[0].position.x, line[0].position.y},
                           {line[1].position.x, line[1].position.y},
                           ImU32(color.toInteger()), 1.0f);
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
   */
  void setPixel(int x, int y, float z, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size)
  {
    int index = y * static_cast<int>(window_size.x) + x;

    if (z < z_buffer[index])
    {
      z_buffer[index] = z;
      color_buffer[index] = color;
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
  void DrawBuffer(ImDrawList *draw_list, const std::vector<float> &z_buffer, const std::vector<sf::Color> &color_buffer, core::Vector2 window_size)
  {
    int width = static_cast<int>(window_size.x);
    int height = static_cast<int>(window_size.y);

    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        int index = y * width + x;

        draw_list->AddRectFilled({static_cast<float>(x), static_cast<float>(y)}, {static_cast<float>(x + 1), static_cast<float>(y + 1)}, color_buffer[index].toInteger());
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
  void DrawVertexBuffer(ImDrawList *draw_list, const core::Vertex &vertex, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size)
  {
    int x = static_cast<int>(vertex.getX(true));
    int y = static_cast<int>(vertex.getY(true));
    int index = y * static_cast<int>(window_size.x) + x;

    if (vertex.getZ() < z_buffer[index])
    {
      for (int i = -2; i < 3; i++)
      {
        for (int j = -2; j < 3; j++)
        {
          setPixel(x + i, y + j, vertex.getZ(), color, z_buffer, color_buffer, window_size);
        }
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
   */
  void DrawLineBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> &vertexes, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size)
  {
    int vertex_length = vertexes.size();
    for (size_t i = 0; i < vertex_length - 1; i++)
    {
      // Se o vértice for válido
      if (vertexes[i].x != -1 && vertexes[i + 1].x != -1)
      {
        int x0 = static_cast<int>(vertexes[i].x);
        int y0 = static_cast<int>(vertexes[i].y);
        int x1 = static_cast<int>(vertexes[i + 1].x);
        int y1 = static_cast<int>(vertexes[i + 1].y);

        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
          setPixel(x0, y0, 0, color, z_buffer, color_buffer, window_size);

          if (x0 == x1 && y0 == y1)
            break;

          int e2 = 2 * err;
          if (e2 > -dy)
          {
            err -= dy;
            x0 += sx;
          }
          if (e2 < dx)
          {
            err += dx;
            y0 += sy;
          }
        }
      }
    }
  }

} // namespace utils