#include <gui/view/components/hierarquical_viewer.hpp>

namespace GUI
{

  /**
   * @brief Construtor da classe HierarchyViewer
   *
   * @param _controller Referência para o controlador da aplicação
   */
  GUI::components::HierarchyViewer::HierarchyViewer(GUI::Controller *_controller)
  {
    this->controller = _controller;
  }

  /**
   * @brief Função que renderiza um nó da árvore de hierarquia, quando o objeto é uma malha (objeto 3D).
   *
   * @param object Ponteiro para o objeto a ser renderizado
   * @param index Índice do objeto na lista de objetos da cena
   *
   */
  void GUI::components::HierarchyViewer::objectRenderingNode(models::Mesh *object, int index)
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (this->selected_index == index)
    {
      node_flags |= ImGuiTreeNodeFlags_Selected;
      this->controller->selectObject(object);
    }

    bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, object->getName().c_str());

    if (ImGui::IsItemClicked())
      this->selected_index = index;

    static bool show_material_popup = false;
    static int popup_object_index = -1;

    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Selecionar"))
      {
        this->selected_index = index;
        this->controller->selectObject(object);
      }
      if (ImGui::MenuItem("Remover"))
      {
        this->controller->removeObject(object);
        this->selected_index = -1;
      }

      if (ImGui::MenuItem("Editar Material")) // Abre o popup ao clicar
      {
        show_material_popup = true;
        popup_object_index = index;
      }

      ImGui::EndPopup();
    }

    if (show_material_popup && popup_object_index == index)
    {
      ImGui::OpenPopup("Editar Material");
    }

    if (ImGui::BeginPopupModal("Editar Material", &show_material_popup))
    {
      {

        ImGui::Text("Iluminação ambiente:");
        ImGui::ColorEdit3("##ambient", reinterpret_cast<float *>(&object->material.ambient));

        ImGui::Text("Iluminação difusa:");
        ImGui::ColorEdit3("##diffuse", reinterpret_cast<float *>(&object->material.diffuse));

        ImGui::Text("Iluminação especular:");
        ImGui::ColorEdit3("##specular", reinterpret_cast<float *>(&object->material.specular));

        ImGui::Text("Brilho:");
        ImGui::InputFloat("##shininess", &object->material.shininess);

        if (ImGui::Button("Fechar"))
        {
          show_material_popup = false;
          ImGui::CloseCurrentPopup();
        }
      }

      ImGui::EndPopup();
    }

    if (node_open)
      ImGui::TreePop();
  }

  /**
   * @brief Função que renderiza um nó da árvore de hierarquia, quando o objeto é uma camera.
   *
   * @param camera Ponteiro para a câmera a ser renderizada
   * @param index Índice da câmera na lista de câmeras da cena
   *
   */
  void GUI::components::HierarchyViewer::cameraRenderingNode(models::Camera3D *camera)
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    static bool show_camera_properties_popup = false;

    bool node_open = ImGui::TreeNodeEx((void *)"camera_node", node_flags, "câmera"); // ID fixo para o nó

    if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) // Verifica se o nó foi clicado duas vezes
    {
      show_camera_properties_popup = true;
    }

    if (show_camera_properties_popup)
    {
      ImGui::OpenPopup("Propriedades da Câmera");
    }

    if (ImGui::BeginPopupModal("Propriedades da Câmera", &show_camera_properties_popup))
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

      if (ImGui::Button("Fechar"))
      {
        show_camera_properties_popup = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    if (node_open)
    {
      ImGui::TreePop();
    }
  }

  void GUI::components::HierarchyViewer::render(models::Scene *scene)
  {
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    ImGui::Begin("Hierarquia da cena", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    cameraRenderingNode(scene->getCamera());

    // Flags para o nó da árvore
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    for (int i = 0; i < scene->getObjects().size(); i++)
    {
      auto object = scene->getObjects()[i];

      this->objectRenderingNode(object, i);
    }
    ImGui::End();
  }

} // namespace GUI