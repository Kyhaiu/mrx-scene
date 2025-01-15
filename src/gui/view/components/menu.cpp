#include <gui/view/components/menu.hpp>

namespace GUI
{

  /**
   * @brief Renderiza o menu da aplicação
   *
   * @param controller Referência para o controlador da aplicação
   * @param fileDialog Referência para o diálogo de arquivos
   *
   * @note O menu da aplicação é responsável por renderizar as opções
   * relacionadas ao arquivo e a cena
   */
  void GUI::components::menu(GUI::Controller *controller, ImGui::FileBrowser &fileDialog)
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Arquivos"))
      {
        if (ImGui::MenuItem("Novo"))
        {
          controller->newScene();
        }
        if (ImGui::MenuItem("Abrir"))
        {

          fileDialog.SetTitle("Abrir arquivo");
          fileDialog.SetTypeFilters({".json"});

          fileDialog.Open();
        }
        if (ImGui::MenuItem("Salvar"))
        {
          controller->save_scene();
        }
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
      if (ImGui::BeginMenu("Configurações da cena"))
      {
        if (ImGui::BeginMenu("Modelo de iluminação"))
        {
          // radio button para escolher o modelo de iluminação

          ImGui::RadioButton("Flat Shading", &controller->getScene()->lighting_model, 0);
          ImGui::RadioButton("Gouraud Shading", &controller->getScene()->lighting_model, 1);
          ImGui::RadioButton("Phong Shading", &controller->getScene()->lighting_model, 2);

          ImGui::EndMenu();
        }

        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
} // namespace GUI