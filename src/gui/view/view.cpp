#include <gui/view/view.hpp>

#include <iostream>

GUI::UI::UI(sf::RenderWindow &_window) : window(_window)
{
  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

  float canvasWidth = (desktopMode.width * 0.9f) * 0.8f;
  float canvasHeight = (desktopMode.height * 0.9f);

  this->controller = new GUI::Controller(canvasWidth, canvasHeight);

  this->hierarchyViewer = new GUI::components::HierarchyViewer(this->controller);

  this->controller->updateScene();

  this->window.setFramerateLimit(30);
  this->window.setVerticalSyncEnabled(true); // Disable VSync
  // ImGui::SFML::Init(_window);
  if (!ImGui::SFML::Init(window))
    return;

  // TODO: Arrumar o jeito de carregar os assets
  if (!this->move_icon.loadFromFile("assets/icons/move_icon.png"))
  {
    std::cerr << "Error loading move icon" << std::endl;
  }
}

GUI::UI::~UI()
{
  ImGui::SFML::Shutdown();
}

/**
 * @brief Retorna a janela da aplicação
 *
 * @return sf::RenderWindow&
 */
sf::RenderWindow &GUI::UI::getWindow()
{
  return this->window;
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
  GUI::components::menu(this->controller);
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
  GUI::components::object_inspector(this->controller->getScene());
}

void GUI::UI::render()
{
  this->menu();

  ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.2f, ImGui::GetWindowSize().y));
  ImGui::Begin("left-container", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  ImGui::SetNextWindowPos(ImVec2(0, 20));
  ImGui::BeginChild("hierarchy", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y / 2), false);
  this->hierarchy(this->controller->getScene());
  ImGui::EndChild();

  ImGui::BeginChild("actions", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y / 2), false);
  this->object_properties();
  ImGui::EndChild();

  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x * 0.2f, 20));
  ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.8f, ImGui::GetWindowSize().y));
  ImGui::Begin("viewport", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  // std::cout << "Content Region: " << ImGui::GetContentRegionAvail().x << " " << ImGui::GetContentRegionAvail().y << std::endl;
  // std::cout << "Window Size: " << ImGui::GetWindowSize().x << " " << ImGui::GetWindowSize().y << std::endl;

  this->viewport(this->controller->getScene());

  ImGui::End();

  this->controller->updateScene();

  // ImGui::SetNextWindowPos(ImVec2((windowSize.x / 2) - 100, windowSize.y * 0.85f));
  // this->objectActions();

  // // ImGui::SetNextWindowPos(ImVec2(0, 20 + canvasHeight));
  // // ImGui::SetNextWindowSize(ImVec2(windowWidth, terminalHeight));
  // // this->terminal();
}

/**
 * @brief Manipula os eventos do mouse e do teclado
 *
 * @param event Evento do mouse ou do teclado
 * @param window Janela da aplicação
 */
void GUI::UI::handleEvents(const sf::Event &event, sf::RenderWindow &window)
{
  ImGui::SFML::ProcessEvent(window, event);

  // ImGuiIO &io = ImGui::GetIO();

  // std::cout << "Mouse Pos: " << io.MousePos.x << " " << io.MousePos.y << std::endl;
  this->controller->handleEvents(event, window);
}