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

    utils::DrawBuffer(draw_list, scene->z_buffer, scene->color_buffer, scene->getMinViewport());
  }

} // namespace GUI