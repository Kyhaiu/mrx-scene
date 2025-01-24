#include <gui/view/components/components.hpp>

#include <iostream>

namespace GUI
{

  // void GUI::components::object_inspector(GUI::Controller *controller)
  // {

  //   ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetContentRegionAvail().y + 16));
  //   ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
  //   ImGui::Begin("object-properties-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  //   if (ImGui::BeginTabBar("Propriedades do objeto"))
  //   {
  //     if (ImGui::BeginTabItem("Propriedades do objeto"))
  //     {
  //       if (controller->getScene()->getSelectedObject() != nullptr)
  //       {
  //         components::object_inspector(controller->getScene()->getSelectedObject());
  //       }
  //       // auto camera = controller->getScene()->getCamera();

  //       // auto min_window = controller->getScene()->getMinWindow();
  //       // auto max_window = controller->getScene()->getMaxWindow();
  //       // auto min_viewport = controller->getScene()->getMinViewport();
  //       // auto max_viewport = controller->getScene()->getMaxViewport();

  //       // ImGui::InputFloat2("Min Janela", reinterpret_cast<float *>(&min_window));
  //       // ImGui::InputFloat2("Max Janela", reinterpret_cast<float *>(&max_window));

  //       // ImGui::InputFloat2("Min Viewport", reinterpret_cast<float *>(&min_viewport));
  //       // ImGui::InputFloat2("Max Viewport", reinterpret_cast<float *>(&max_viewport));

  //       // ImGui::InputFloat3("Posição", reinterpret_cast<float *>(&camera->position));
  //       // ImGui::InputFloat3("Alvo", reinterpret_cast<float *>(&camera->target));

  //       // ImGui::SliderAngle("Angulo de rotação", &controller->camera_rotation_sensitivity, 0.0f, 360.0f);

  //       // ImGui::InputFloat3("Posição da luz", reinterpret_cast<float *>(&controller->getScene()->omni_lights[0].position));

  //       // // ImGui::SliderFloat3("Posição da Luz", reinterpret_cast<float *>(&controller->getScene()->omni_lights[0]), -100.0f, 100.0f);

  //       // // show the fps
  //       // ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

  //       // ImGui::InputFloat("D", reinterpret_cast<float *>(&camera->d));
  //       // ImGui::InputFloat("Near", reinterpret_cast<float *>(&camera->near));
  //       // ImGui::InputFloat("Far", reinterpret_cast<float *>(&camera->far));

  //       // // models::CameraYaw(scene->getCamera(), *f * 3.14159, false);
  //       ImGui::EndTabItem();
  //     }
  //   }

  //   ImGui::End();
  // }

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

      if (object == scene->getSelectedObject())
      {
        core::Vector4 box = object->getBox(true);
        utils::DrawBoundingBox({box.x, box.y}, {box.z, box.w}, models::YELLOW, scene->z_buffer, scene->color_buffer);
      }

      core::Vector3 object_centroid = {0.0f, 0.0f, 0.0f};

      if (scene->centroid_algorithm == CENTROID_BY_WRAP_BOX)
        object_centroid = object->getCentroidByWrapBox();
      else if (scene->centroid_algorithm == CENTROID_BY_MEAN)
        object_centroid = object->getCentroidByMean();

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

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        // O vetor normal da face é calculado na ocultação de faces
        // precisa recortar o vetor normal do vertice também (assim simplifica o calculo de interpolação)
        // usar excel como base

        if (scene->lighting_model == FLAT_SHADING)
          utils::DrawFaceBufferFlatShading(vertexes, scene->getCamera()->position, face->getFaceCentroid(), face->getNormal(), object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);
        else if (scene->lighting_model == GOURAUD_SHADING)
          utils::DrawFaceBufferGouraudShading(vertexes, scene->getCamera()->position, object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);
        else if (scene->lighting_model == PHONG_SHADING)
          utils::DrawFaceBufferPhongShading(vertexes, object_centroid, scene->getCamera()->position, object->material, scene->global_light, scene->omni_lights, scene->z_buffer, scene->color_buffer);

        // Limpa o vetor de vetores normais dos vértices da face
        vertexes.clear();
      }
    }

    // Desenha no buffer a luz omni
    // utils::DrawVertexBuffer(scene->omni_lights[0].screen_position, models::ChannelsToColor(scene->omni_lights[0].intensity), scene->z_buffer, scene->color_buffer, 20);

    utils::DrawBuffer(draw_list, scene->z_buffer, scene->color_buffer, min_viewport);
  }

} // namespace GUI