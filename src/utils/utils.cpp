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

    // Como o buffer não tem as dimensões da janela, é necessário definir um tamanho mínimo
    int min_x = static_cast<int>(min_window_size.x);
    int min_y = static_cast<int>(min_window_size.y);

    ImGui::SetCursorScreenPos(ImVec2(min_window_size.x, min_window_size.y));

    for (int x = min_x; x < width; x++)
    {
      // min_y + 20 é para que ele comece a desenhar abaixo do titulo da janela (viewport)
      for (int y = min_y + 20; y < height; y++)
      {
        if (!models::CompareColors(color_buffer[x][y], models::TRANSPARENT))
          draw_list->AddRectFilled({static_cast<float>(x), static_cast<float>(y)}, {static_cast<float>(x + 1), static_cast<float>(y + 1)}, GET_COLOR_UI32(color_buffer[x][y]));
      }
    }
  }

  /**
   * @brief Desenha um vértice no buffer
   *
   * @param point Ponto a ser desenhado
   * @param size Tamanho do ponto
   * @param color Cor do ponto
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   * @todo Ajustar o calculo do z_buffer para esta função
   */
  void DrawVertexBuffer(const core::Vector3 point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, const int size = 3)
  {
    int x = static_cast<int>(point.x);
    int y = static_cast<int>(point.y);

    for (float i = -2; i < size; i++)
    {
      for (float j = -2; j < size; j++)
      {
        setPixel(x + i, y + j, point.z, color, z_buffer, color_buffer);
      }
    }
  }

  /**
   * @brief Desenha uma linha no buffer
   *
   * @param vertexes Vetor de vértices que compõem a linha
   * @param color Cor da linha
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   * @note O Algoritmo implementado é o do Bresenham adaptado para interpolação de Z também.
   */
  void DrawLineBuffer(const std::vector<core::Vector3> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    ImDrawList *draw_list = ImGui::GetForegroundDrawList();
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
   * @param vertexes Vetor de vértices que compõem a face
   * @param eye Posição do observador
   * @param face_centroid Centroide da face
   * @param face_normal Vetor normal da face
   * @param object_material Material do objeto
   * @param global_light Luz global
   * @param omni_lights Vetor de luzes omni
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   * @todo Arrumar bug de preenchimento
   */
  void DrawFaceBufferFlatShading(const std::vector<std::pair<core::Vector3, core::Vector3>> &vertexes, const core::Vector3 &eye, const core::Vector3 &face_centroid, const core::Vector3 &face_normal, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    math::fill_polygon_flat_shading(vertexes, global_light, omni_lights, eye, face_centroid, face_normal, object_material, z_buffer, color_buffer, {static_cast<float>(color_buffer.size()), static_cast<float>(color_buffer[0].size())});
  }

  /**
   * @brief Desenha uma face no buffer utilizando Gouraud Shading
   *
   * @param vertexes Vetor de vértices e normais dos vertices que compõem a face
   * @param eye Posição do observador
   * @param face_normal Vetor normal da face
   * @param object_material Material do objeto
   * @param global_light Luz global
   * @param omni_lights Vetor de luzes omni
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   *
   */
  void DrawFaceBufferGouraudShading(const std::vector<std::pair<core::Vector3, core::Vector3>> &vertexes, const core::Vector3 &eye, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {

    math::fill_polygon_gourand(vertexes, global_light, omni_lights, eye, object_material, z_buffer, color_buffer);
  }

  /**
   * @brief Desenha uma face no buffer utilizando Phong Shading
   *
   * @param vertexes Vetor de vértices e normais dos vertices que compõem a face
   * @param centroid Centroide da face
   * @param eye Posição do observador
   * @param object_material Material do objeto
   * @param global_light Luz global
   * @param omni_lights Vetor de luzes omni
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   */
  void DrawFaceBufferPhongShading(const std::vector<std::pair<core::Vector3, core::Vector3>> &vertexes, const core::Vector3 &centroid, const core::Vector3 &eye, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    math::fill_polygon_phong(vertexes, centroid, global_light, omni_lights, eye, object_material, z_buffer, color_buffer);
  }

  /**
   * @brief Desenha um texto na janela
   *
   * @param text Texto a ser desenhado
   * @param position Posição do texto
   * @param color Cor do texto
   */
  void DrawString(const char *text, const core::Vector3 &position, const models::Color &color)
  {
    ImDrawList *draw_list = ImGui::GetForegroundDrawList();

    draw_list->AddText(ImVec2(position.x, position.y), GET_COLOR_UI32(color), text);
  }

  /**
   * @brief Desenha a caixa de seleção
   *
   * @param min_point Ponto mínimo da caixa
   * @param max_point Ponto máximo da caixa
   * @param color Cor da caixa
   * @param z_buffer Buffer de profundidade
   * @param color_buffer Buffer de cores
   */
  void DrawBoundingBox(const core::Vector2 &min_point, const core::Vector2 &max_point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
  {
    // Vetor de vértices que compõem a caixa
    // Obs.: O valor de z é -99999 para que a caixa seja desenhada na frente de todos os objetos
    std::vector<core::Vector3> vertexes = {
        // Linha superior
        {min_point.x, min_point.y, -99999.f},
        {max_point.x, min_point.y, -99999.f},
        // Linha inferior
        {max_point.x, max_point.y, -99999.f},
        {min_point.x, max_point.y, -99999.f},
        // Linha vertical esquerda
        {min_point.x, min_point.y, -99999.f},
        {min_point.x, max_point.y, -99999.f},
        // Linha vertical direita
        {max_point.x, min_point.y, -99999.f},
        {max_point.x, max_point.y, -99999.f},
    };

    DrawLineBuffer(vertexes, color, z_buffer, color_buffer);
  }

} // namespace utils