#include <gui/view/components/object_inspector.hpp>

namespace GUI
{
  void components::objectInspector(GUI::Controller *controller)
  {
    models::Mesh *object = controller->getScene()->getSelectedObject();

    if (controller->element_selected_type == GUI::ItemSelected::OBJECT)
    {
      object = controller->getScene()->getObjects()[controller->selected_element_index];
      objectInspector(object, controller);
    }
    else if (controller->element_selected_type == GUI::ItemSelected::OMNI_LIGHT)
    {
      models::Omni *omni_light = &controller->getScene()->omni_lights[controller->selected_element_index];
      objectInspector(omni_light, controller);
    }
  }

  void components::objectInspector(models::Camera3D *camera)
  {

    ImGui::Text("Posição:");
    ImGui::InputFloat3("##position", &camera->position.x);

    ImGui::Text("Foco:");
    ImGui::InputFloat3("##target", &camera->target.x);

    ImGui::Text("D:");
    ImGui::InputFloat("##d", &camera->d);

    ImGui::Text("Near:");
    ImGui::InputFloat("##near", &camera->near);

    ImGui::Text("Far:");
    ImGui::InputFloat("##far", &camera->far);
  }

  void components::objectInspector(models::Light *light)
  {
    ImGui::Text("Intensidade:");
    ImGui::ColorEdit3("##intensity", reinterpret_cast<float *>(&light->intensity));
  }

  void components::objectInspector(models::Mesh *object, GUI::Controller *controller)
  {
    if (controller->getScene()->getSelectedObject() == nullptr)
      return;

    // Pega o nome do objeto & converte para char*
    std::string name = object->getName();
    char nameBuffer[256];
    strncpy_s(nameBuffer, name.c_str(), sizeof(nameBuffer));
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
    object->setName(nameBuffer);

    if (ImGui::TreeNode("Material"))
    {
      ImGui::Text("Ka:");
      ImGui::ColorEdit3("##ambient", reinterpret_cast<float *>(&object->material.ambient));

      ImGui::Text("Kd:");
      ImGui::ColorEdit3("##diffuse", reinterpret_cast<float *>(&object->material.diffuse));

      ImGui::Text("Ks:");
      ImGui::ColorEdit3("##specular", reinterpret_cast<float *>(&object->material.specular));

      ImGui::Text("Brilho:");
      ImGui::InputFloat("##shininess", &object->material.shininess);

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Translação"))
    {
      static core::Vector3 translation = {0.0f, 0.0f, 0.0f}; // Variável estática para armazenar a translação *desejada*

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f)); // Valores HSV entre 0 e 1
      ImGui::Button("X");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##position-x", &translation.x)) // Verifica se o valor foi alterado
        controller->translate_object(object, translation);

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 1.0f, 1.0f)); // Valores HSV entre 0 e 1 (0.33 para verde)
      ImGui::Button("Y");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##position-y", &translation.y)) // Verifica se o valor foi alterado
        controller->translate_object(object, translation);

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.66f, 1.0f, 1.0f)); // Valores HSV entre 0 e 1 (0.66 para azul)
      ImGui::Button("Z");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##position-z", &translation.z)) // Verifica se o valor foi alterado
        controller->translate_object(object, translation);

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Rotação"))
    {
      static core::Vector3 rotation = {0.0f, 0.0f, 0.0f};

      const char *labels[] = {"X", "Y", "Z"};
      for (int i = 0; i < 3; ++i)
      {
        ImGui::PushID(i);                                                                    // Importante para IDs únicos
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i * 0.33f, 1.0f, 1.0f)); // Cores HSV corretas
        ImGui::Button(labels[i]);
        ImGui::PopStyleColor(1);
        ImGui::SameLine();

        float rotation_degrees = rotation[i] * 180.0f / PI;                                                               // Converte radianos para graus para o slider
        if (ImGui::DragFloat(("##rotation-" + std::string(labels[i])).c_str(), &rotation_degrees, 1.0f, -360.0f, 360.0f)) // Slider em graus
        {
          rotation[i] = rotation_degrees * PI / 180.0f; // Converte de volta para radianos
          controller->rotate_object(object, rotation);
        }
        ImGui::PopID();
      }

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Escala"))
    {
      static core::Vector3 scale = {1.0f, 1.0f, 1.0f};

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
      ImGui::Button("X");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##scale-x", &scale.x))
        controller->scale_object(object, scale);

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 1.0f, 1.0f));
      ImGui::Button("Y");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##scale-y", &scale.y))
        controller->scale_object(object, scale);

      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.66f, 1.0f, 1.0f));
      ImGui::Button("Z");
      ImGui::PopStyleColor(1);
      ImGui::SameLine();
      if (ImGui::InputFloat("##scale-z", &scale.z))
        controller->scale_object(object, scale);

      ImGui::TreePop();
    }
  }

  void components::objectInspector(models::Omni *omni_light, GUI::Controller *controller)
  {
    if (controller->getScene()->selected_omni_light == nullptr)
      return;

    ImGui::Text("Posição");

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
    ImGui::Button("X");
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::InputFloat("##position-x", &omni_light->position.x);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 1.0f, 1.0f));
    ImGui::Button("Y");
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::InputFloat("##position-y", &omni_light->position.y);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.66f, 1.0f, 1.0f));
    ImGui::Button("Z");
    ImGui::PopStyleColor(1);
    ImGui::SameLine();
    ImGui::InputFloat("##position-z", &omni_light->position.z);

    ImGui::Text("Intensidade:");
    ImGui::ColorEdit3("##intensity", reinterpret_cast<float *>(&omni_light->intensity));
  }
} // namespace GUI