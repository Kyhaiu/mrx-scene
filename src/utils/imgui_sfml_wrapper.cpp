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
  }

  /**
   * @brief Desenha uma linha na janela
   *
   * @param window Referência para a janela onde a linha será desenhada
   * @param v1 Vértice inicial da linha
   * @param v2 Vértice final da linha
   * @param color Cor da linha
   */
  void DrawLine(ImDrawList *draw_list, const core::Vertex &v1, const core::Vertex &v2, const sf::Color &color)
  {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(v1.getX(true), v1.getY(true))),
        sf::Vertex(sf::Vector2f(v2.getX(true), v2.getY(true)))};
    draw_list->AddLine({line[0].position.x, line[0].position.y},
                       {line[1].position.x, line[1].position.y},
                       ImU32(color.toInteger()), 1.0f);
  }
} // namespace utils