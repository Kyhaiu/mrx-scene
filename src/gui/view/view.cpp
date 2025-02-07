#include <gui/view/view.hpp>

#include <iostream>

#undef near
#undef far

GUI::UI::UI(SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer)
{

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
  ImGui_ImplSDLRenderer2_Init(this->renderer);

  // Get the window size SDL2
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  float window_width = static_cast<float>(dm.w);
  float window_height = static_cast<float>(dm.h);

  // Setup controller
  this->controller = new GUI::Controller(window_width, window_height);

  this->hierarchyViewer = new GUI::components::HierarchyViewer(this->controller);

  this->controller->updateScene();
}

/**
 * @brief Destrutor da classe UI
 */
GUI::UI::~UI()
{
  // Cleanup
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

/**
 * @brief Renderiza a área de visualização da cena
 *
 * @note A área de visualização da cena é responsável por renderizar a cena
 * em si
 */
void GUI::UI::viewport(models::Scene *_scene)
{
  GUI::components::viewport(_scene);
}

/**
 * @brief Rendeniza o menu principal da aplicação
 *
 * @note O menu principal da aplicação é responsável por renderizar as opções
 * relacionadas ao arquivo e a cena
 */
void GUI::UI::menu()
{
  GUI::components::menu(this->controller, this->fileDialog);
}

void GUI::UI::hierarchy(models::Scene *_scene)
{
  this->hierarchyViewer->render(_scene);
}

void GUI::UI::sidebar()
{
}

/**
 * @brief Mostra o menu de inserção de objetos na cena
 *
 * @note O menu de inserção de objetos na cena é responsável por conter as
 * opções de inserção de objetos na cena
 * @note A variável showInsertOptions é responsável por controlar a visibilidade
 */
void GUI::UI::insertOptions()
{
}

/**
 * @brief Renderiza as ações relacionadas aos objetos da cena
 *
 * @note As ações relacionadas aos objetos da cena são responsáveis por
 * renderizar as opções de manipulação dos objetos da cena
 */
void GUI::UI::object_properties()
{
  ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetContentRegionAvail().y + 16));
  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
  ImGui::Begin("object-properties-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  if (ImGui::BeginTabBar("Propriedades do objeto"))
  {
    if (ImGui::BeginTabItem("Propriedades do objeto"))
    {
      if (this->controller->element_selected_type == GUI::ItemSelected::OBJECT)
        GUI::components::objectInspector(this->controller);
      if (this->controller->element_selected_type == GUI::ItemSelected::OMNI_LIGHT)
        GUI::components::objectInspector(this->controller);
      else if (this->controller->element_selected_type == GUI::ItemSelected::CAMERA)
        GUI::components::objectInspector(this->controller->getScene()->getCamera());
      else if (this->controller->element_selected_type == GUI::ItemSelected::GLOBAL_LIGHT)
        GUI::components::objectInspector(&this->controller->getScene()->global_light);

      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }

  ImGui::End();
}

/**
 * @brief Renderiza a interface gráfica
 *
 * @note A interface gráfica é responsável por renderizar todos os componentes
 * da aplicação
 */
void GUI::UI::render()
{

  // Start the Dear ImGui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui::NewFrame();

  this->menu();

  // Rendeniza o diálogo de arquivos, caso ele tenha sido aberto no menu
  this->fileDialog.Display();
  models::Camera3D *camera = this->controller->getScene()->getCamera();

  if (this->controller->benchmarking)
  {
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(this->controller->windowWidth), static_cast<float>(this->controller->windowHeight)));
    ImGui::Begin("benchmark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

    if (camera->position == core::Vector3{0.0f, 0.0f, 0.0f})
    {
      if (this->controller->type_camera_movement == ORBITAL_MOVEMENT)
      {
        this->controller->type_camera_movement = FREE_MOVEMENT;
        // Re centraliza a camera no eixo Z
        //  Arredonda pra baixo pra eliminar casa decimais que a rotação deixou
        camera->position = {0.0f, 0.0f, 0.0f};
      }
      else
        this->controller->type_camera_movement = ORBITAL_MOVEMENT;
    }

    this->controller->update_camera_benchmark();

    core::Vector3 vrp = camera->position;
    core::Vector3 eye = camera->target;

    // Container com 20% da largura da tela
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(this->controller->windowWidth * 0.2f, ImGui::GetWindowSize().y));
    ImGui::Begin("left-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Desenha os fps na tela
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::YELLOW)), "FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Text("Pipeline: ");
    ImGui::SameLine();
    if (this->controller->getScene()->pipeline_model == SMITH_PIPELINE)
      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::CYAN)), "SMITH");
    else
      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::CYAN)), "ADAIR");

    ImGui::Text("VRP");
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::RED)), "X: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(vrp.x));
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::BLUE)), "Y: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(vrp.y));
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::GREEN)), "Z: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(vrp.z));

    ImGui::Text("Eye");
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::RED)), "X: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(eye.x));
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::BLUE)), "Y: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(eye.y));
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::GREEN)), "Z: ");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(eye.z));

    ImGui::Text("Near: ");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::MAGENTA)), "%d", (int)camera->near);
    ImGui::Text("Far: ");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::MAGENTA)), "%d", (int)camera->far);

    // padding de 50px verticalmente
    ImGui::Dummy(ImVec2(0, 50));

    // Grafico do tempo de rasterização
    // Converte os doubles para float (necessário para PlotLines)
    static std::vector<float> frame_times_float;
    frame_times_float.clear();
    for (double time : this->controller->benchmark_results.frame_times)
    {
      frame_times_float.push_back(static_cast<float>(time));
    }

    char overlay[32];
    snprintf(
        overlay,
        sizeof(overlay),
        "Frame Times (ms) - Total (%d)",
        static_cast<int>(this->controller->benchmark_results.total_frames) // Converte para int
    );

    float available_width = ImGui::GetContentRegionAvail().x;
    // Plot do gráfico
    ImGui::PlotLines(
        "",                                         // Label do gráfico
        frame_times_float.data(),                   // Dados (array de floats)
        static_cast<int>(frame_times_float.size()), // Número de pontos
        0,                                          // Offset para início dos dados
        overlay,                                    // Rótulo overlay (opcional)
        0.0f,                                       // Escala mínima (0 = auto)
        FLT_MAX,                                    // Escala máxima (FLT_MAX = auto)
        ImVec2(available_width, 150)                // Tamanho do gráfico (largura, altura)
    );

    // Mostra estatísticas adicionais (opcional)
    if (!this->controller->benchmark_results.frame_times.empty())
    {
      double current_time = this->controller->benchmark_results.frame_times.back();
      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::YELLOW)), "Current frame time:");
      ImGui::SameLine();
      ImGui::Text("%.4f ms", current_time);
      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::YELLOW)), "Average frame time:");
      ImGui::SameLine();
      ImGui::Text("%.4f ms", this->controller->benchmark_results.average_frame_time * 1000);

      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::GREEN)), "Min frame time:");
      ImGui::SameLine();
      ImGui::Text("%.4f ms", this->controller->benchmark_results.min_frame_time);
      ImGui::TextColored(ImColor(models::GET_COLOR_UI32(models::RED)), "Max frame time:");
      ImGui::SameLine();
      ImGui::Text("%.4f ms", this->controller->benchmark_results.max_frame_time);
    }

    ImGui::Dummy(ImVec2(0, 50));

    // Grafico dos 10% piores frames

    // Converte os doubles para float (necessário para PlotLines)
    static std::vector<float> worst_10_percentile_float;
    worst_10_percentile_float.clear();
    for (double time : this->controller->benchmark_results.worst_10_percentile)
    {
      worst_10_percentile_float.push_back(static_cast<float>(time));
    }

    char overlay_2[64];
    snprintf(
        overlay_2,
        sizeof(overlay_2),
        "Worst 10%% Frame Times (ms) - Total (%d)",
        static_cast<int>(this->controller->benchmark_results.worst_10_percentile.size()) // Converte para int
    );

    // Plot do gráfico
    ImGui::PlotLines(
        "",                                                 // Label do gráfico
        worst_10_percentile_float.data(),                   // Dados (array de floats)
        static_cast<int>(worst_10_percentile_float.size()), // Número de pontos
        0,                                                  // Offset para início dos dados
        overlay_2,                                          // Rótulo overlay (opcional)
        0.0f,                                               // Escala mínima (0 = auto)
        FLT_MAX,                                            // Escala máxima (FLT_MAX = auto)
        ImVec2(available_width, 150)                        // Tamanho do gráfico (largura, altura)
    );

    ImGui::End();

    // Mede o tempo de rasterização
    auto start_time = std::chrono::high_resolution_clock::now();
    this->controller->updateScene();
    this->viewport(this->controller->getScene());
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> frame_time = end_time - start_time;

    // Atualiza o benchmark com o tempo do frame
    this->controller->update_benchmark(frame_time.count());

    ImGui::End();

    if (vrp.z <= -192.0f)
      this->controller->end_benchmark();
  }
  else
  {
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(this->controller->windowWidth * 0.2f, ImGui::GetWindowSize().y));
    ImGui::Begin("left-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(this->controller->windowWidth * 0.2f, this->controller->windowHeight * 0.5f));
    ImGui::BeginChild("hierarchy", ImVec2(this->controller->windowWidth * 0.2f, this->controller->windowHeight * 0.5f), false);
    this->hierarchy(this->controller->getScene());
    ImGui::EndChild();

    ImGui::SetNextWindowPos(ImVec2(0, this->controller->windowHeight * 0.5f));
    ImGui::SetNextWindowSize(ImVec2(this->controller->windowWidth * 0.2f, this->controller->windowHeight * 0.5f));
    ImGui::BeginChild("actions", ImVec2(this->controller->windowWidth * 0.2f, this->controller->windowHeight * 0.5f), false);
    this->object_properties();
    ImGui::EndChild();

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2((float)this->controller->windowWidth * 0.2f, 20));
    ImGui::SetNextWindowSize(ImVec2((float)this->controller->windowWidth * 0.8f, (float)this->controller->windowHeight));
    ImGui::Begin("viewport", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    this->controller->updateScene();
    this->viewport(this->controller->getScene());

    ImGui::End();
  }

  // Rendering
  ImGui::Render();
  SDL_SetRenderDrawColor(this->renderer, 114, 144, 154, 255);
  SDL_RenderClear(this->renderer);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), this->renderer);
  SDL_RenderPresent(this->renderer);
}

/**
 * @brief Manipula os eventos do mouse e do teclado
 *
 * @param event Evento do mouse ou do teclado
 * @param window Janela da aplicação
 * @param renderer Renderizador da aplicação
 */
void GUI::UI::handleEvents(const SDL_Event &event, SDL_Window *window, SDL_Renderer *renderer)
{
  // Process ImGui events using SDL2
  ImGui_ImplSDL2_ProcessEvent(&event);

  // Convert SDL time to seconds
  Uint32 currentTicks = SDL_GetTicks();
  float deltaTime = static_cast<float>(currentTicks) / 1000.0f;

  // Pass the event and time to controller
  this->controller->handleEvents(event, window, deltaTime);

  // handle file dialog events
  if (this->fileDialog.HasSelected())
  {
    this->controller->on_file_dialog_open(this->fileDialog.GetSelected().string());
    this->fileDialog.ClearSelected();
  }
}
