#include <gui/view/components/components.hpp>

#include <iostream>

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

  /**
   * @brief Renderiza o menu da aplicação
   *
   * @param controller Referência para o controlador da aplicação
   *
   * @note O menu da aplicação é responsável por renderizar as opções
   * relacionadas ao arquivo e a cena
   */
  void GUI::components::menu(GUI::Controller *controller)
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Arquivos"))
      {
        ImGui::MenuItem("Novo");
        ImGui::MenuItem("Abrir");
        ImGui::MenuItem("Salvar");
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Inserir"))
      {
        if (ImGui::MenuItem("Cubo"))
          controller->addObject(shapes::cube());
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Editar"))
      {
        ImGui::MenuItem("Desfazer");
        ImGui::MenuItem("Refazer");
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Configurações"))
      {
        ImGui::MenuItem("Preferências");
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void GUI::components::object_inspector(GUI::Controller *controller)
  {
    // std::cout << "Object options" << std::endl;
    // std::cout << "Content Region: " << ImGui::GetContentRegionAvail().x << " " << ImGui::GetContentRegionAvail().y << std::endl;
    // std::cout << "Window Size: " << ImGui::GetWindowSize().x << " " << ImGui::GetWindowSize().y << std::endl;

    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetContentRegionAvail().y + 15));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    ImGui::Begin("object-properties-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::BeginTabBar("Propriedades do objeto"))
    {
      if (ImGui::BeginTabItem("Propriedades"))
      {

        auto camera = controller->getScene()->getCamera();

        ImGui::InputFloat3("Posição", reinterpret_cast<float *>(&camera->position));
        ImGui::InputFloat3("Alvo", reinterpret_cast<float *>(&camera->target));

        ImGui::SliderAngle("Angulo de rotação", &controller->camera_rotation_sensitivity, 0.0f, 360.0f);

        // models::CameraYaw(scene->getCamera(), *f * 3.14159, false);
        ImGui::EndTabItem();
      }
    }

    ImGui::End();
  }

  void GUI::components::viewport(models::Scene *scene)
  {
    // ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    // ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y - ImGui::GetFrameHeight()));
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // this can be used to get the position of the window
    // ImVec2 window_pos = ImGui::GetWindowPos();

    // std::cout << "Window Pos:" << window_pos.x << " " << window_pos.y << std::endl;

    core::HalfEdge *he = nullptr;

    for (auto object : scene->getObjects())
    {
      for (auto face : object->getFaces())
      {

        if (!face->getVisible())
          continue;

        he = face->getHalfEdge();
        while (true)
        {
          utils::DrawVertex(draw_list, *he->getOrigin());

          object->isSelected()
              ? utils::DrawLine(draw_list, *he->getOrigin(), *he->getNext()->getOrigin(), sf::Color::Red)
              : utils::DrawLine(draw_list, *he->getOrigin(), *he->getNext()->getOrigin(), sf::Color::White);

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        draw_list->AddText(ImVec2(face->getFaceCentroid(true).x, face->getFaceCentroid(true).y), sf::Color::Red.toInteger(), face->getId().c_str());
      }
    }
  }

  //-----------------------------------------------------------------------------------------------
  // HierarchyViewer Class
  //-----------------------------------------------------------------------------------------------

  GUI::components::HierarchyViewer::HierarchyViewer(GUI::Controller *_controller)
  {
    this->controller = _controller;
  }

  void GUI::components::HierarchyViewer::render(models::Scene *scene)
  {
    // std::cout << "Hierarchy" << std::endl;
    // std::cout << "Content Region: " << ImGui::GetContentRegionAvail().x << " " << ImGui::GetContentRegionAvail().y << std::endl;
    // std::cout << "Window Size: " << ImGui::GetWindowSize().x << " " << ImGui::GetWindowSize().y << std::endl;

    ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    ImGui::Begin("hierarchy-component-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    if (ImGui::TreeNode("Hierarquia da cena"))
    {
      if (ImGui::TreeNodeEx((void *)(intptr_t)selected_index, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "Camera"))
      {
        if (ImGui::IsItemClicked())
        {
          selected_index = 0;
          this->controller->on_hierarchy_item_selected(-1);
        }
        else
        {
          selected_index = -1;
        }
      }
    }
    ImGui::TreePop();
    ImGui::End();
  }
  //-----------------------------------------------------------------------------------------------
  // End of HierarchyViewer Class
  //-----------------------------------------------------------------------------------------------
} // namespace GUI