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
  void DrawLine(ImDrawList *draw_list, const std::vector<core::Vector2> vertexes, const sf::Color &color)
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
} // namespace utils