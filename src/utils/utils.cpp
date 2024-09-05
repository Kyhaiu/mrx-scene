#include <utils/utils.hpp>
#include <iostream>

namespace utils
{
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
   * @note Esta função é um wrapper para a função z_buffer
   */
  void setPixel(const float x, const float y, const float z, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    math::z_buffer(x, y, z, color, z_buffer, color_buffer);
  }

  /**
   * @brief Desenha um buffer na janela
   *
   * @param draw_list Referência para a janela onde o buffer será desenhado
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   */
  void DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<models::Color>> &color_buffer, core::Vector2 min_window_size)
  {
    int width = color_buffer.size();
    int height = color_buffer[0].size();

    ImGui::SetCursorScreenPos(ImVec2(min_window_size.x, min_window_size.y));

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        if (!models::CompareColors(color_buffer[x][y], models::TRANSPARENT))
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

   * @todo Ajustar o calculo do z_buffer para esta função
   */
  void DrawVertexBuffer(ImDrawList *draw_list, const core::Vector3 point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    int x = static_cast<int>(point.x);
    int y = static_cast<int>(point.y);

    for (float i = -2; i < 3; i++)
    {
      for (float j = -2; j < 3; j++)
      {
        setPixel(x + i, y + j, -9999, color, z_buffer, color_buffer);
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
   *
   * @note O Algoritmo implementado é o do Bresenham adaptado para interpolação de Z também.
   */
  void DrawLineBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    int vertex_length = vertexes.size();
    for (size_t i = 0; i < vertex_length - 1; i = i + 2)
    {
      // Se o vértice for válido
      if (vertexes[i].x != -1 && vertexes[i + 1].x != -1)
      {
        std::vector<core::Vector3> line = math::BresenhamLine(vertexes[i], vertexes[i + 1]);

        for (auto vertex : line)
        {
          setPixel(vertex.x, vertex.y, vertex.z, color, z_buffer, color_buffer);
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
  void DrawFaceBuffer(const std::vector<core::Vector3> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    math::fill_polygon(vertexes, color, z_buffer, color_buffer, {static_cast<float>(color_buffer.size()), static_cast<float>(color_buffer[0].size())});
  }

} // namespace utils