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

    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetContentRegionAvail().y + 16));
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

        // show the fps
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::InputFloat("D", reinterpret_cast<float *>(&camera->d));

        // models::CameraYaw(scene->getCamera(), *f * 3.14159, false);
        ImGui::EndTabItem();
      }
    }

    ImGui::End();
  }

  void GUI::components::viewport(models::Scene *scene)
  {

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();

    // this can be used to get the position of the window
    // ImVec2 window_pos = ImGui::GetWindowPos();

    core::HalfEdge *he = nullptr;

    core::Vector2 min_viewport = scene->getMinViewport();
    core::Vector2 max_viewport = scene->getMaxViewport();

    std::pair<core::Vector3, core::Vector3> clipped_vertex;
    scene->initializeBuffers();

    std::vector<SDL_Color> face_colors = {{255, 30, 150, 255}, {40, 255, 80, 255}, {90, 40, 255, 255}};

    for (auto object : scene->getObjects())
    {
      int color_picking = 0;
      for (auto face : object->getFaces())
      {

        if (!face->getVisible())
          continue;

        he = face->getHalfEdge();

        // Necessário limpar o vetor pois a cada frame ele é preenchido novamente
        // TODO: Descobrir uma maneira de não precisar limpar o vetor se não houver mudanças
        face->clipped_vertex.clear();

        std::vector<core::Vector3> face_vertexes;

        while (true)
        {

          clipped_vertex = math::clip_line(he->getOrigin()->getVectorScreen(), he->getNext()->getOrigin()->getVectorScreen(), min_viewport, max_viewport);
          face->clipped_vertex.push_back(clipped_vertex.first);
          face->clipped_vertex.push_back(clipped_vertex.second);

          utils::DrawVertexBuffer(draw_list, *he->getOrigin(), {255, 255, 255, 255}, scene->z_buffer, scene->color_buffer, max_viewport);

          face_vertexes.push_back(he->getOrigin()->getVectorScreen());

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        object->isSelected()
            ? utils::DrawLineBuffer(draw_list, face->clipped_vertex, {255, 0, 0, 255}, scene->z_buffer, scene->color_buffer, max_viewport)
            : utils::DrawLineBuffer(draw_list, face->clipped_vertex, {255, 255, 255, 255}, scene->z_buffer, scene->color_buffer, max_viewport);

        color_picking++;

        object->isSelected()
            ? utils::DrawFaceBuffer(draw_list, face->clipped_vertex, face_colors[color_picking], scene->z_buffer, scene->color_buffer, max_viewport)
            : utils::DrawFaceBuffer(draw_list, face->clipped_vertex, face_colors[color_picking], scene->z_buffer, scene->color_buffer, max_viewport);
      }
    }

    utils::DrawBuffer(draw_list, scene->z_buffer, scene->color_buffer, max_viewport);
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
    ImGui::SetNextWindowPos(ImVec2(0, 20));
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