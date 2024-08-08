#include <gui/controller/controller.hpp>
#include <iostream>

GUI::Controller::Controller(float canvasWidth, float canvasHeight)
{
  this->scene = new models::Scene(
      models::CreateCamera3D({10, 10, 20}, {1, 1, 2}, {0, 1, 0}, 18),
      {},
      // The canvas don't start at the left corner
      // If the layout changes it will be necessary to change this values
      {canvasWidth * 0.2f, 20},
      {canvasWidth, canvasHeight},
      {-3, -3},
      {3, 3});
}

/**
 * @brief Destrutor da classe Controller
 *
 */
GUI::Controller::~Controller()
{
}

/**
 * @brief Retorna o ponteiro para o modelo da cena 3D
 *
 * @return models::Scene*
 */
models::Scene *GUI::Controller::getScene()
{
  return this->scene;
}

/**
 * @brief Define o modelo da cena 3D
 *
 * @param _scene Referência para o modelo da cena 3D
 */
void GUI::Controller::setScene(models::Scene &_scene)
{
  this->scene = &_scene;
}

/**
 * @brief Atualiza a cena caso haja alguma alteração
 *
 */
void GUI::Controller::updateScene()
{
  // models::CameraOrbital(this->scene->getCamera(), this->camera_rotation_sensitivity);

  this->scene->rasterize();
}

/**
 * @brief Adiciona um objeto à cena
 *
 * @note A cena é atualizada após a adição do objeto
 *
 * @param object Ponteiro para o objeto a ser adicionado
 */
void GUI::Controller::addObject(models::Mesh *object)
{
  this->scene->addObject(object);
  this->updateScene();
}

//-----------------------------------------------------------------------------
// Events
//-----------------------------------------------------------------------------
/**
 * @brief Manipula os eventos do mouse e do teclado
 *
 * @param event Evento do mouse ou do teclado
 * @param window Janela da aplicação
 * @param deltaTime Tempo entre frames
 */
void GUI::Controller::handleEvents(const sf::Event &event, sf::RenderWindow &window, sf::Time deltaTime)
{
  if (event.type == sf::Event::Closed)
  {
    window.close();
  }
  else if (event.type == sf::Event::Resized)
  {
    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
    window.setView(sf::View(visibleArea));
  }
  else if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      this->isMouseHeld = true;                                 // Mouse button is held down
      this->lastMousePosition = sf::Mouse::getPosition(window); // Get the initial position of the mouse
      if (this->scene->getSelectedObject() == nullptr)
      {
        this->scene->selectObject(event.mouseButton.x, event.mouseButton.y);
      }
    }
  }
  else if (event.type == sf::Event::MouseButtonReleased)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      this->isMouseHeld = false; // Mouse button is released
    }
  }
  else if (event.type == sf::Event::MouseMoved)
  {
    // TODO - Arrumar a ação de mover o objeto para somente mover quando o mouse estiver dentro do objeto, caso contrário rotacionar a camera
    // Se o mouse está pressionado e não há objeto selecionado, então rotaciona a camera
    if (this->isMouseHeld && this->scene->getSelectedObject() == nullptr)
    {
      // Update the mouse position
      this->mousePosition = sf::Mouse::getPosition(window);

      // Get the deltas that describe how much the mouse got moved between frames
      float deltaAngleX = ((2 * M_PI) / this->scene->getMaxViewport().x); // a movement from left to right = 2*PI = 360 deg
      float deltaAngleY = (M_PI / this->scene->getMaxViewport().y);       // a movement from top to bottom = PI = 180 deg
      float dx = (this->lastMousePosition.x - this->mousePosition.x) * deltaAngleX;
      float dy = (this->lastMousePosition.y - this->mousePosition.y) * deltaAngleY;

      models::CameraArcball(this->scene->getCamera(), dx, dy);
      // Optionally reset start position to allow continuous rotation
      this->lastMousePosition = this->mousePosition;
    }
    else if (this->isMouseHeld && this->scene->getSelectedObject() != nullptr)
    {
      // Update the mouse position
      this->mousePosition = sf::Mouse::getPosition(window);

      // Get the deltas that describe how much the mouse got moved between frames
      float dx = (this->lastMousePosition.x - this->mousePosition.x);
      float dy = (this->lastMousePosition.y - this->mousePosition.y);

      this->scene->translateObject({-dx, -dy, 0});

      // Optionally reset start position to allow continuous rotation
      this->lastMousePosition = this->mousePosition;
    }
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::D && (event.key.control))
    {
      this->scene->deselectObject();
    }
    else if (event.key.code == sf::Keyboard::W)
    {
      models::CameraMoveForward(this->scene->getCamera(), 0.09f, true);
    }
    else if (event.key.code == sf::Keyboard::S)
    {
      models::CameraMoveForward(this->scene->getCamera(), -0.09f, true);
    }
    else if (event.key.code == sf::Keyboard::A)
    {
      models::CameraMoveRight(this->scene->getCamera(), 0.09f, true);
    }
    else if (event.key.code == sf::Keyboard::D)
    {
      models::CameraMoveRight(this->scene->getCamera(), -0.09f, true);
    }
  }
}

/**
 * @brief Callback para quando um item da hierarquia é selecionado
 *
 * @param index Índice do item selecionado
 *
 * @note Este método é chamado quando um item da hierarquia é selecionado
 * @note O índice -1 é reservado para a câmera
 * @note O índice -2 é reservado para a luz
 */
void GUI::Controller::on_hierarchy_item_selected(int index)
{
  if (index == -1)
  {
    // Rise the flag to show that the camera is selected
    this->camera_is_selected = true;
  }
  else if (index == -2)
  {
    this->camera_is_selected = false;
  }
  else
  {
    this->camera_is_selected = false;

    int i = 0;

    for (auto object : this->scene->getObjects())
    {
      if (i == index)
      {
        this->scene->setSelectedObject(object);
        break;
      }
      i++;
    }
  }
}