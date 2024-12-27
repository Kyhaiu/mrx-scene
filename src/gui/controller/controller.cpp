#include <gui/controller/controller.hpp>
#include <iostream>

GUI::Controller::Controller(float canvasWidth, float canvasHeight)
{
  this->windowHeight = static_cast<int>(canvasHeight);
  this->windowWidth = static_cast<int>(canvasWidth);

  this->scene = new models::Scene(
      models::CreateCamera3D({10, 10, 20}, {0, 0, 0}, {0, 1, 0}, 20),
      {},
      // The canvas don't start at the left corner
      // If the layout changes it will be necessary to change this values
      {canvasWidth * 0.2f, 20},
      {canvasWidth, canvasHeight},
      {-3, -3},
      {3, 3});

  this->insertionOptions = {1, 1.0f, 1.0f, 10, 10};
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

  this->scene->rasterize_adair_pipeline();
  // this->scene->rasterize_portugues_pipeline();
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
void GUI::Controller::handleEvents(const SDL_Event &event, SDL_Window *window, float deltaTime)
{

  switch (event.type)
  {
  case SDL_QUIT:
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
    break;

  case SDL_WINDOWEVENT:
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      // Update the viewport
      int width = event.window.data1;
      int height = event.window.data2;
      this->windowHeight = height;
      this->windowWidth = width;
      SDL_RenderSetViewport(SDL_GetRenderer(window), NULL);
    }
    break;

  case SDL_MOUSEBUTTONDOWN:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      this->isMouseHeld = true;
      SDL_GetMouseState(&this->lastMousePosition.x, &this->lastMousePosition.y);

      if (this->scene->getSelectedObject() == nullptr)
      {
        this->scene->selectObject(event.button.x, event.button.y);
      }
    }
    break;

  case SDL_MOUSEBUTTONUP:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      this->isMouseHeld = false;
    }
    break;

  case SDL_MOUSEMOTION:

    if (this->isMouseHeld && this->scene->getSelectedObject() == nullptr)
    {
      SDL_GetMouseState(&this->mousePosition.x, &this->mousePosition.y);

      float deltaAngleX = (2 * PI) / this->scene->getMaxViewport().x;
      float deltaAngleY = PI / this->scene->getMaxViewport().y;
      float dx = (this->lastMousePosition.x - this->mousePosition.x) * deltaAngleX;
      float dy = (this->lastMousePosition.y - this->mousePosition.y) * deltaAngleY;

      // Previne a hiper-rotação da câmera
      float cosAngle = math::Vector3DotProduct(this->scene->getCamera()->target, this->scene->getCamera()->up);
      if (cosAngle * ((deltaAngleY > 0) - (deltaAngleY < 0)) > 0.0f)
        dy = 0;

      models::CameraArcball(this->scene->getCamera(), dy, dx);
      this->lastMousePosition = this->mousePosition;
    }
    else if (this->isMouseHeld && this->scene->getSelectedObject() != nullptr)
    {
      SDL_GetMouseState(&this->mousePosition.x, &this->mousePosition.y);

      float dx = static_cast<float>(this->lastMousePosition.x - this->mousePosition.x);
      float dy = static_cast<float>(this->lastMousePosition.y - this->mousePosition.y);

      if (SDL_GetModState() & KMOD_CTRL)
        this->scene->translateObject({-dx, 0, -dy});
      else
        this->scene->translateObject({-dx, -dy, 0});

      this->lastMousePosition = this->mousePosition;
    }
    break;

  case SDL_KEYDOWN:
    switch (event.key.keysym.sym)
    {
    case SDLK_d:
      if (event.key.keysym.mod & KMOD_CTRL)
      {
        this->scene->deselectObject();
      }
      else
      {
        models::CameraMoveRight(this->scene->getCamera(), -0.09f, true);
      }
      break;
    case SDLK_w:
      models::CameraMoveForward(this->scene->getCamera(), 0.09f, true);
      break;
    case SDLK_s:
      models::CameraMoveForward(this->scene->getCamera(), -0.09f, true);
      break;
    case SDLK_a:
      models::CameraMoveRight(this->scene->getCamera(), 0.09f, true);
      break;
    }
    break;

  case SDL_MOUSEWHEEL:
    if (event.wheel.y > 0) // scroll up
    {
      models::Camera3D *camera = this->scene->getCamera();
      camera->d -= 0.1f;
    }
    else if (event.wheel.y < 0) // scroll down
    {
      models::Camera3D *camera = this->scene->getCamera();
      camera->d += 0.1f;
    }
    break;
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