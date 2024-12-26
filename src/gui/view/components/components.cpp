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
        if (ImGui::MenuItem("Pirâmide"))
          controller->addObject(shapes::pyramid());
        if (ImGui::BeginMenu("Esfera"))
        {
          if (ImGui::BeginMenu("Esfera"))
          {
            ImGui::SliderFloat("Raio", &controller->insertionOptions.radius, 0.1f, 3.0f);
            ImGui::SliderInt("Segmentos", &controller->insertionOptions.segments, 3, 100);
            ImGui::SliderInt("Anéis", &controller->insertionOptions.rings, 3, 100);

            if (ImGui::Button("Criar Esfera"))
            {
              controller->addObject(shapes::sphere(controller->insertionOptions.radius, controller->insertionOptions.rings, controller->insertionOptions.segments));
            }

            ImGui::EndMenu();
          }

          if (ImGui::BeginMenu("Icosfera"))
          {
            ImGui::SliderFloat("Raio", &controller->insertionOptions.radius, 0.1f, 3.0f);
            ImGui::SliderInt("Subdivisões", &controller->insertionOptions.subdivisions, 1, 5);

            if (ImGui::Button("Criar"))
            {
              controller->addObject(shapes::icosphere(controller->insertionOptions.radius, controller->insertionOptions.subdivisions));
            }
            ImGui::EndMenu();
          }

          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Cone"))
        {

          ImGui::SliderFloat("Raio", &controller->insertionOptions.radius, 0.1f, 3.0f);
          ImGui::SliderFloat("Altura", &controller->insertionOptions.height, 0.1f, 3.0f);
          ImGui::SliderInt("Segmentos", &controller->insertionOptions.segments, 3, 100);

          if (ImGui::Button("Criar"))
          {
            controller->addObject(shapes::cone(controller->insertionOptions.radius, controller->insertionOptions.height, controller->insertionOptions.segments));
          }

          ImGui::EndMenu();
        }
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

        auto min_window = controller->getScene()->getMinWindow();
        auto max_window = controller->getScene()->getMaxWindow();
        auto min_viewport = controller->getScene()->getMinViewport();
        auto max_viewport = controller->getScene()->getMaxViewport();

        ImGui::InputFloat2("Min Janela", reinterpret_cast<float *>(&min_window));
        ImGui::InputFloat2("Max Janela", reinterpret_cast<float *>(&max_window));

        ImGui::InputFloat2("Min Viewport", reinterpret_cast<float *>(&min_viewport));
        ImGui::InputFloat2("Max Viewport", reinterpret_cast<float *>(&max_viewport));

        ImGui::InputFloat3("Posição", reinterpret_cast<float *>(&camera->position));
        ImGui::InputFloat3("Alvo", reinterpret_cast<float *>(&camera->target));

        ImGui::SliderAngle("Angulo de rotação", &controller->camera_rotation_sensitivity, 0.0f, 360.0f);

        ImGui::InputFloat3("Posição da luz", reinterpret_cast<float *>(&controller->getScene()->omni_lights[0].position));

        // ImGui::SliderFloat3("Posição da Luz", reinterpret_cast<float *>(&controller->getScene()->omni_lights[0]), -100.0f, 100.0f);

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

    // Não existe objetos para desenhar
    if (scene->getObjects().size() == 0)
      return;

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();

    // this can be used to get the position of the window
    // ImVec2 window_pos = ImGui::GetWindowPos();

    core::HalfEdge *he = nullptr;

    core::Vector2 min_viewport = scene->getMinViewport();
    core::Vector2 max_viewport = scene->getMaxViewport();

    // Evitar que durante a rasterização o programa tente acessar uma posição inválida
    max_viewport.x = max_viewport.x - 1;
    max_viewport.y = max_viewport.y - 1;

    std::pair<core::Vector3, core::Vector3> clipped_vertex;
    scene->initializeBuffers();

    for (auto object : scene->getObjects())
    {
      for (auto face : object->getFaces())
      {

        if (!face->getVisible())
          continue;

        he = face->getHalfEdge();

        // Necessário limpar o vetor pois a cada frame ele é preenchido novamente
        // TODO: Descobrir uma maneira de não precisar limpar o vetor se não houver mudanças
        face->clipped_vertex.clear();

        // Vetor que armazena os vetores normais médios dos vértices da face
        std::vector<core::Vertex *> vertexes;

        while (true)
        {

          clipped_vertex = math::clip_line(he->getOrigin()->getVectorScreen(), he->getNext()->getOrigin()->getVectorScreen(), min_viewport, max_viewport);
          face->clipped_vertex.push_back(clipped_vertex.first);
          face->clipped_vertex.push_back(clipped_vertex.second);

          vertexes.push_back(he->getOrigin());
          vertexes.push_back(he->getNext()->getOrigin());

          // utils::DrawVertexBuffer(clipped_vertex.first, models::WHITE, scene->z_buffer, scene->color_buffer, 5);
          // utils::DrawString(he->getOrigin()->getId().c_str(), clipped_vertex.first, models::RED);

          // utils::DrawLineBuffer({clipped_vertex.first, clipped_vertex.second}, models::WHITE, scene->z_buffer, scene->color_buffer);

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        // utils::DrawFaceBufferFlatShading(vertexes, scene->getCamera()->position, face->getFaceCentroid(), face->getNormal(), object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);
        // O vetor normal da face é calculado na ocultação de faces
        // precisa recortar o vetor normal do vertice também (assim simplifica o calculo de interpolação)
        // usar excel como base
        // utils::DrawFaceBufferGouraudShading(vertexes, scene->getCamera()->position, object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);
        utils::DrawFaceBufferPhongShading(vertexes, face->getFaceCentroid(), scene->getCamera()->position, object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);

        // utils::DrawString(face->getId().c_str(), face->getFaceCentroid(true), models::WHITE);

        // Limpa o vetor de vetores normais dos vértices da face
        vertexes.clear();
      }
    }

    // Desenha no buffer a luz omni
    // utils::DrawVertexBuffer(scene->omni_lights[0].screen_position, models::ChannelsToColor(scene->omni_lights[0].intensity), scene->z_buffer, scene->color_buffer, 20);

    utils::DrawBuffer(draw_list, scene->z_buffer, scene->color_buffer, min_viewport);
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