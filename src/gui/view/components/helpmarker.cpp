#include <gui/view/components/helpmarker.hpp>

namespace GUI
{
  /**
   * @brief Renderiza um marcador de ajuda (?) que exibe uma descrição ao passar o mouse sobre ele
   *
   * @param desc Descrição do marcador de ajuda
   *
   */
  void GUI::components::HelpMarker(const char *desc)
  {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }
} // namespace GUI