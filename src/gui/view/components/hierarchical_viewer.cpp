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

    if (this->selected_index == index && this->controller->element_selected_type == GUI::ItemSelected::OBJECT)
    {
      node_flags |= ImGuiTreeNodeFlags_Selected;
      this->controller->selectObject(object);
    }

    bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, object->getName().c_str());

    if (ImGui::IsItemClicked())
    {
      if (this->selected_index == index)
      {
        this->selected_index = -1;

        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::NONE, -1);
      }
      else
      {
        this->selected_index = index;
        this->controller->selectObject(object);
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::OBJECT, index);
      }
    }

    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Selecionar"))
      {
        this->selected_index = index;
        this->controller->selectObject(object);
      }
      if (ImGui::MenuItem("Deselecionar"))
      {
        this->selected_index = -1;
        this->controller->selectObject(nullptr);
      }
      if (ImGui::MenuItem("Remover"))
      {
        this->controller->removeObject(object);
        this->selected_index = -1;
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::NONE, -1);
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

    if (this->controller->selected_element_index == -2)
    {
      node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool node_open = ImGui::TreeNodeEx((void *)"camera_node", node_flags, "câmera"); // ID fixo para o nó

    if (ImGui::IsItemClicked())
    {
      if (this->selected_index == -2)
      {
        this->selected_index = -1;
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::NONE, -1);
      }
      else
      {
        this->selected_index = -2;
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::CAMERA, -2);
      }
    }

    if (node_open)
      ImGui::TreePop(); // Adicione esta linha
  }

  /**
   * @brief Função que renderiza um nó da árvore de hierarquia, quando o objeto é uma luz.
   *
   * @param light
   */
  void components::HierarchyViewer::lightRenderingNode(models::Light *light)
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    bool node_open = ImGui::TreeNodeEx((void *)"light_node", node_flags, "luz global"); // ID fixo para o nó

    if (this->controller->selected_element_index == -3)
    {
      node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::IsItemClicked())
    {
      if (this->selected_index == -3)
      {
        this->selected_index = -1;

        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::NONE, -1);
      }
      else
      {
        this->selected_index = -3;
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::GLOBAL_LIGHT, -3);
      }
    }

    if (node_open)
      ImGui::TreePop(); // Adicione esta linha
  }

  void components::HierarchyViewer::omnidirectionalLightRenderingNode(models::Omni *light, int index)
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (this->selected_index == index && this->controller->element_selected_type == GUI::ItemSelected::OMNI_LIGHT)
    {
      node_flags |= ImGuiTreeNodeFlags_Selected;
      this->controller->getScene()->selected_omni_light = light;
    }

    bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, "luz omnidirecional"); // ID fixo para o nó

    if (ImGui::IsItemClicked())
    {
      if (this->selected_index == index)
      {
        this->selected_index = -1;

        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::NONE, -1);
      }
      else
      {
        this->selected_index = index;
        this->controller->on_hierarchy_item_selected(GUI::ItemSelected::OMNI_LIGHT, index);
      }
    }

    if (node_open)
      ImGui::TreePop(); // Adicione esta linha
  }

  void GUI::components::HierarchyViewer::render(models::Scene *scene)
  {
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    ImGui::Begin("Hierarquia da cena", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    cameraRenderingNode(scene->getCamera());

    lightRenderingNode(&scene->global_light);

    for (int i = 0; i < scene->omni_lights.size(); i++)
    {
      auto light = &scene->omni_lights[i];

      this->omnidirectionalLightRenderingNode(light, i);
    }

    for (int i = 0; i < scene->getObjects().size(); i++)
    {
      auto object = scene->getObjects()[i];

      this->objectRenderingNode(object, i);
    }
    ImGui::End();
  }

} // namespace GUI