#include <gui/controller/controller.hpp>
#include <iostream>

GUI::Controller::Controller(float canvasWidth, float canvasHeight)
{
  this->windowHeight = static_cast<int>(canvasHeight);
  this->windowWidth = static_cast<int>(canvasWidth);

  this->scene = new models::Scene(
      models::CreateCamera3D({10, 10, 20}, {0, 0, 0}, {0, 1, 0}, 30, 20, 40),
      {},
      // The canvas don't start at the left corner
      // If the layout changes it will be necessary to change this values
      {canvasWidth * 0.2f, 0},
      {canvasWidth, canvasHeight},
      {-2, -2},
      {2, 2});

  this->insertionOptions = {1, 1.0f, 1.0f, 10, 10, 1.0f, 0.5f, 100, 100};
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

  if (this->scene->pipeline_model == SANTA_CATARINA_PIPELINE)
    this->scene->adair_pipeline();
  else
    this->scene->smith_pipeline();
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

/**
 * @brief Remove um objeto da cena
 *
 * @note A cena é atualizada após a remoção do objeto
 *
 * @param object Ponteiro para o objeto a ser removido
 */
void GUI::Controller::removeObject(models::Mesh *object)
{
  this->scene->removeObject(object);
  this->updateScene();
}

/**
 * @brief Seleciona um objeto da cena
 *
 * @param object Ponteiro para o objeto a ser selecionado
 */
void GUI::Controller::selectObject(models::Mesh *object)
{
  this->scene->setSelectedObject(object);

  if (object != nullptr)
  {
    this->previousRotation[object] = object->rotation;
    this->previousScale[object] = object->scale;
    this->previousTranslations[object] = object->position;
  }
}

/**
 * @brief Cria uma nova cena
 *
 */
void GUI::Controller::newScene()
{

  float canvasWidth = static_cast<float>(this->windowWidth);
  float canvasHeight = static_cast<float>(this->windowHeight);

  this->scene = new models::Scene(
      models::CreateCamera3D({10, 10, 20}, {0, 0, 0}, {0, 1, 0}, 30, 20, 40),
      {},
      // The canvas don't start at the left corner
      // If the layout changes it will be necessary to change this values
      {canvasWidth * 0.2f, 20},
      {canvasWidth, canvasHeight},
      {-3, -3},
      {3, 3});

  this->insertionOptions = {1, 1.0f, 1.0f, 10, 10};
}

void GUI::Controller::translate_object(models::Mesh *object, core::Vector3 translation)
{

  // Obtém a translação anterior (se existir) ou inicializa com zero
  core::Vector3 previousTranslation = this->previousTranslations.count(object) ? this->previousTranslations[object] : core::Vector3{0, 0, 0};

  // Calcula a translação *real* a ser aplicada (diferença entre a nova e a anterior)
  core::Vector3 actualTranslation = {
      translation.x - previousTranslation.x,
      translation.y - previousTranslation.y,
      translation.z - previousTranslation.z};

  this->getScene()->translateObject(actualTranslation);

  // Atualiza a posição do objeto diretamente
  object->position.x += actualTranslation.x;
  object->position.y += actualTranslation.y;
  object->position.z += actualTranslation.z;

  // Atualiza a translação anterior
  previousTranslations[object] = translation;
}

void GUI::Controller::rotate_object(models::Mesh *object, core::Vector3 rotation)
{
  core::Vector3 actualRotation = {
      rotation.x - (this->previousRotation.count(object) ? previousRotation[object].x : 0.0f),
      rotation.y - (previousRotation.count(object) ? previousRotation[object].y : 0.0f),
      rotation.z - (previousRotation.count(object) ? previousRotation[object].z : 0.0f)};

  this->getScene()->rotateObject(actualRotation);

  object->rotation.x = rotation.x;
  object->rotation.y = rotation.y;
  object->rotation.z = rotation.z;

  previousRotation[object] = rotation;
}

void GUI::Controller::scale_object(models::Mesh *object, core::Vector3 scale)
{
  core::Vector3 actualScale = {
      scale.x - (this->previousScale.count(object) ? previousScale[object].x : 0.0f),
      scale.y - (previousScale.count(object) ? previousScale[object].y : 0.0f),
      scale.z - (previousScale.count(object) ? previousScale[object].z : 0.0f)};

  this->getScene()->scaleObject(actualScale);

  object->scale.x = scale.x;
  object->scale.y = scale.y;
  object->scale.z = scale.z;

  previousScale[object] = scale;
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
        models::CameraMoveRight(this->scene->getCamera(), -1.0f, true);
      }
      break;
    case SDLK_w:
      models::CameraMoveForward(this->scene->getCamera(), 1.0f, true);
      break;
    case SDLK_s:
      models::CameraMoveForward(this->scene->getCamera(), -1.0f, true);
      break;
    case SDLK_a:
      models::CameraMoveRight(this->scene->getCamera(), 1.0f, true);
      break;
    }
    break;

  case SDL_MOUSEWHEEL:

    if (this->scene->getSelectedObject() != nullptr)
    {
      // Verifica se há um objeto selecionado
      if (this->scene->getSelectedObject() != nullptr)
      {
        // Obtém a direção do scroll
        int scrollY = event.wheel.y;

        // Define a sensibilidade do scroll (ajuste conforme necessário)
        float sensitivity = 1.0f;

        // Aplica a translação no eixo Z
        this->scene->translateObject({0, 0, static_cast<float>(-scrollY) * sensitivity});
      }
      break;
    }
    else
    {

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
}

/**
 * @brief Callback para quando um item da hierarquia é selecionado
 *
 * @param item_type Tipo do item selecionado
 * @param object_index Índice do objeto selecionado
 *
 * @note Este método é chamado quando um item da hierarquia é selecionado
 */
void GUI::Controller::on_hierarchy_item_selected(int item_type, int object_index)
{
  if (item_type == GUI::ItemSelected::NONE)
  {
    this->selected_element_index = -1;
    this->scene->setSelectedObject(nullptr);
    this->element_selected_type = GUI::ItemSelected::NONE;
  }
  else if (item_type == GUI::ItemSelected::OBJECT)
  {
    this->selected_element_index = object_index;
    this->scene->setSelectedObject(this->scene->getObjects()[object_index]);
    this->element_selected_type = GUI::ItemSelected::OBJECT;
  }
  else if (item_type == GUI::ItemSelected::OMNI_LIGHT)
  {
    this->element_selected_type = GUI::ItemSelected::OMNI_LIGHT;
    this->selected_element_index = object_index;
    this->scene->selected_omni_light = &this->scene->omni_lights[object_index];
  }
  else if (item_type == GUI::ItemSelected::CAMERA)
  {
    this->element_selected_type = GUI::ItemSelected::CAMERA;
    this->selected_element_index = object_index;
  }
  else if (item_type == GUI::ItemSelected::GLOBAL_LIGHT)
  {
    this->element_selected_type = GUI::ItemSelected::GLOBAL_LIGHT;
    this->selected_element_index = object_index;
  }
}

/**
 * @brief Callback para quando o diálogo de arquivos é aberto
 *
 * @param file Caminho do arquivo selecionado
 */
void GUI::Controller::on_file_dialog_open(const std::string &file)
{

  json j = utils::load_json(file);

  this->scene->from_json(j["scene"]);

  this->updateScene();
}

/**
 * @brief Salva a cena em um arquivo
 *
 */
void GUI::Controller::save_scene()
{
  json j;

  j["scene"] = this->scene->to_json();

  std::string filename = "scene.json";

  utils::save_json(filename, j);
}

void GUI::Controller::start_benchmark()
{
  this->benchmarking = true;

  on_file_dialog_open("C:\\Users\\marco\\OneDrive\\Documentos\\TCC\\mrx-scene\\scene.json");

  models::benchmark_start(&this->benchmark_results);
}

void GUI::Controller::end_benchmark()
{
  this->benchmarking = false;

  // Finaliza o benchmark
  models::benchmark_end(&this->benchmark_results);

  std::string filename;
  std::string output_dir = "C:\\Users\\marco\\OneDrive\\Documentos\\TCC\\mrx-scene\\resultados";
  std::string shading_model;
  std::string repetition = std::to_string(this->benchmark_repetitions);

  if (this->getScene()->lighting_model == FLAT_SHADING)
    shading_model = "flat";
  else if (this->getScene()->lighting_model == GOURAUD_SHADING)
    shading_model = "gouraud";
  else
    shading_model = "phong";

  if (this->getScene()->pipeline_model == SMITH_PIPELINE)
  {
    filename = "benchmark_pipeline_smith_" + shading_model + "_results_" + (repetition) + ".txt";
    output_dir += "\\SMITH\\";
  }
  else
  {
    filename = "benchmark_pipeline_adair_" + shading_model + "_results_" + repetition + ".txt";
    output_dir += "\\ADAIR\\";
  }

  if (this->getScene()->lighting_model == FLAT_SHADING)
    output_dir += "\\FLAT\\";
  else if (this->getScene()->lighting_model == GOURAUD_SHADING)
    output_dir += "\\GOURAUD\\";
  else
    output_dir += "\\PHONG\\";

  // Caminho completo
  std::string full_path = output_dir + "\\" + filename;

  // Abre um arquivo para escrita
  std::ofstream outFile(full_path);
  if (!outFile.is_open())
  {
    std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
    return;
  }

  // Escreve os resultados no arquivo
  outFile << "=== Benchmark Results ===\n\n"
          << "Configuration:\n"
          << "Pipeline: " << (this->getScene()->pipeline_model == SMITH_PIPELINE ? "Smith" : "Adair") << "\n"
          << "Shading: " << shading_model << "\n"
          << "Repetitions: " << this->benchmark_repetitions << "\n\n"
          << "Performance Metrics:\n"
          << std::fixed << std::setprecision(4)
          << "Total Time: " << this->benchmark_results.total_time.count() << " s\n"
          << "Total Frames: " << this->benchmark_results.total_frames << "\n"
          << "Average FPS: " << this->benchmark_results.total_frames / this->benchmark_results.total_time.count() << "\n"
          << "Avg Frame Time: " << this->benchmark_results.average_frame_time << " ms\n"
          << "Min Frame Time: " << this->benchmark_results.min_frame_time << " ms\n"
          << "Max Frame Time: " << this->benchmark_results.max_frame_time << " ms\n\n"
          << "Worst 10% Frame Times (ms):\n";

  for (const auto &time : this->benchmark_results.worst_10_percentile)
  {
    outFile << time << "\n";
  }

  outFile.close();
  std::cout << "Results of benchmark " + repetition + " saved to: " << filename << std::endl;
  this->benchmark_repetitions++;

  if (this->benchmark_repetitions <= 10)
  {
    start_benchmark();
  }
}

void GUI::Controller::update_benchmark(double frame_time)
{
  // Adiciona o tempo do frame ao vetor de tempos
  this->benchmark_results.frame_times.push_back(frame_time);

  // Atualiza o tempo total de execução corretamente
  this->benchmark_results.total_time += std::chrono::duration<double>(frame_time);

  // Atualiza o número total de frames
  this->benchmark_results.total_frames++;

  // Atualiza o tempo mínimo e máximo de frame
  if (frame_time < this->benchmark_results.min_frame_time || this->benchmark_results.total_frames == 1)
  {
    this->benchmark_results.min_frame_time = frame_time;
  }
  if (frame_time > this->benchmark_results.max_frame_time)
  {
    this->benchmark_results.max_frame_time = frame_time;
  }

  // Calcula a média de tempo de frame corretamente
  this->benchmark_results.average_frame_time = (this->benchmark_results.total_time.count() / this->benchmark_results.total_frames);

  // Calcula a média de FPS corretamente
  if (this->benchmark_results.total_time.count() > 0)
  {
    this->benchmark_results.average_fps = this->benchmark_results.total_frames / this->benchmark_results.total_time.count();
  }
  else
  {
    this->benchmark_results.average_fps = 0.0;
  }

  // Atualiza os 10% piores frames
  if (this->benchmark_results.frame_times.size() >= 10)
  {
    std::vector<double> sorted_times = this->benchmark_results.frame_times;
    std::sort(sorted_times.begin(), sorted_times.end());
    int worst_count = sorted_times.size() / 10;
    this->benchmark_results.worst_10_percentile.assign(
        sorted_times.end() - worst_count, sorted_times.end());
  }
}

void GUI::Controller::update_camera_benchmark()
{
  models::Camera3D *camera = this->getScene()->getCamera();

  if (this->type_camera_movement == ORBITAL_MOVEMENT)
  {
    models::CameraOrbital(camera, this->camera_rotation_sensitivity);
    accumulatedAngle += this->camera_rotation_sensitivity;

    // Se acumulou 360 graus (2π radianos) ou mais, reinicia o movimento
    if (accumulatedAngle >= 2 * M_PI)
    {
      accumulatedAngle = 0.0f; // reinicia o contador
      this->type_camera_movement = FREE_MOVEMENT;
    }
  }
  else
    models::CameraMoveForward(camera, 1.0f, true);
}