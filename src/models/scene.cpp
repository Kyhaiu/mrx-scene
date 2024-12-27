#include <models/scene.hpp>

namespace models
{
  //------------------------------------------------------------------------------------------------
  // Constructors and Destructors
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Construtor padrão da classe Scene
   *
   * @note Este construtor cria uma câmera padrão e uma lista vazia de objetos
   * @note A câmera padrão é criada com os seguintes parâmetros:
   * @note - posição: (0.0f, 0.0f, 0.0f)
   * @note - alvo: (0.0f, 0.0f, 0.0f)
   * @note - vetor up: (0.0f, 1.0f, 0.0f)
   * @note - d: 1.0f
   * @note - viewport: min(-3.0f, -3.0f) e max(3.0f, 3.0f)
   * @note - window: min(0.0f, 0.0f) e max(100.0f, 100.0f)
   */
  Scene::Scene()
  {
    this->camera = models::CreateCamera3D(
        {0.0f, 0.0f, 20.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        2.0f);
    this->objects = std::vector<models::Mesh *>();
    this->min_viewport = {-3.0f, -3.0f};
    this->max_viewport = {3.0f, 3.0f};
    this->min_window = {0.0f, 0.0f};
    this->max_window = {100.0f, 100.0f};
  }

  /**
   * @brief Construtor da classe Scene
   *
   * @param camera Câmera da cena
   * @param objects Lista de objetos da cena
   * @param min_viewport Coordenadas mínimas da viewport (tela)
   * @param max_viewport Coordenadas máximas da viewport (tela)
   * @param min_window Coordenadas mínimas da janela
   * @param max_window Coordenadas máximas da janela
   */
  Scene::Scene(models::Camera3D *camera, std::vector<models::Mesh *> objects, core::Vector2 min_viewport, core::Vector2 max_viewport, core::Vector2 min_window, core::Vector2 max_window)
  {
    this->camera = camera;
    this->objects = objects;
    this->min_viewport = min_viewport;
    this->max_viewport = max_viewport;
    this->min_window = min_window;
    this->max_window = max_window;
    this->selected_object = nullptr;

    // TODO: Arruma a construção da luz da cena de um jeito mais elegante
    models::Light light;

    light.intensity = models::RED;

    this->global_light = light;

    models::Omni omni;
    omni.position = {2.0f, 2.0f, 2.0f};

    omni.intensity = models::ColorToChannels(models::RED);

    this->omni_lights.push_back(omni);
  }

  /**
   * @brief Destrutor da classe Scene
   *
   * @note Este destrutor libera a memória alocada para a câmera e para os objetos
   */
  Scene::~Scene()
  {
    delete this->camera;
    for (auto object : this->objects)
    {
      delete object;
    }
  }

  //------------------------------------------------------------------------------------------------
  // Getters and Setters
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Retorna a câmera da cena
   *
   * @return Câmera da cena
   */
  models::Camera3D *Scene::getCamera()
  {
    return this->camera;
  }

  /**
   * @brief Retorna a lista de objetos da cena
   *
   * @return Lista de objetos da cena
   */
  std::vector<models::Mesh *> Scene::getObjects()
  {
    return this->objects;
  }

  /**
   * @brief Retorna o objeto selecionado da cena
   *
   * @return Objeto selecionado da cena
   */
  models::Mesh *Scene::getSelectedObject()
  {
    return this->selected_object;
  }

  /**
   * @brief Retorna as coordenadas mínimas da viewport
   *
   * @return Coordenadas mínimas da viewport
   */
  core::Vector2 Scene::getMinViewport()
  {
    return this->min_viewport;
  }

  /**
   * @brief Retorna as coordenadas máximas da viewport
   *
   * @return Coordenadas máximas da viewport
   */
  core::Vector2 Scene::getMaxViewport()
  {
    return this->max_viewport;
  }

  /**
   * @brief Retorna as coordenadas mínimas da janela
   *
   * @return Coordenadas mínimas da janela
   */
  core::Vector2 Scene::getMinWindow()
  {
    return this->min_window;
  }

  /**
   * @brief Retorna as coordenadas máximas da janela
   *
   * @return Coordenadas máximas da janela
   */
  core::Vector2 Scene::getMaxWindow()
  {
    return this->max_window;
  }

  /**
   * @brief Retorna se a cena possui reflexão
   *
   * @return Flag que indica se a cena possui reflexão
   */
  bool Scene::getReflection()
  {
    return this->reflection;
  }

  /**
   * @brief Retorna se a cena foi processada
   *
   * @return Flag que indica se a cena foi processada
   */
  bool Scene::getProcessed()
  {
    return this->processed;
  }

  /**
   * @brief Define a câmera da cena
   *
   * @param camera Ponteiro para a câmera da cena
   */
  void Scene::setCamera(models::Camera3D *camera)
  {
    this->camera = camera;
  }

  /**
   * @brief Define a lista de objetos da cena
   *
   * @param objects Lista de objetos da cena
   */
  void Scene::setObjects(std::vector<models::Mesh *> objects)
  {
    this->objects = objects;
  }

  /**
   * @brief Define o objeto selecionado da cena
   *
   * @param selected_object Ponteiro para o objeto selecionado da cena
   */
  void Scene::setSelectedObject(models::Mesh *selected_object)
  {
    this->selected_object = selected_object;
  }

  /**
   * @brief Define as coordenadas mínimas da viewport
   *
   * @param min_viewport Coordenadas mínimas da viewport
   */
  void Scene::setMinViewport(core::Vector2 min_viewport)
  {
    this->min_viewport = min_viewport;
  }

  /**
   * @brief Define as coordenadas máximas da viewport
   *
   * @param max_viewport Coordenadas máximas da viewport
   */
  void Scene::setMaxViewport(core::Vector2 max_viewport)
  {
    this->max_viewport = max_viewport;
  }

  /**
   * @brief Define as coordenadas mínimas da janela
   *
   * @param min_window Coordenadas mínimas da janela
   */
  void Scene::setMinWindow(core::Vector2 min_window)
  {
    this->min_window = min_window;
  }

  /**
   * @brief Define as coordenadas máximas da janela
   *
   * @param max_window Coordenadas máximas da janela
   */
  void Scene::setMaxWindow(core::Vector2 max_window)
  {
    this->max_window = max_window;
  }

  /**
   * @brief Define se a cena possui reflexão
   *
   * @param reflection Flag que indica se a cena possui reflexão
   */
  void Scene::setReflection(bool reflection)
  {
    this->reflection = reflection;
  }

  /**
   * @brief Define se a cena foi processada
   *
   * @param processed Flag que indica se a cena foi processada
   */
  void Scene::setProcessed(bool processed)
  {
    this->processed = processed;
  }

  //------------------------------------------------------------------------------------------------
  // Functions
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Adiciona um objeto à cena
   *
   * @param object Ponteiro para o objeto a ser adicionado
   */
  void Scene::addObject(models::Mesh *object)
  {
    this->objects.push_back(object);
  }

  /**
   * @brief Remove um objeto da cena
   *
   * @param object Ponteiro para o objeto a ser removido
   */
  void Scene::removeObject(models::Mesh *object)
  {
    for (auto it = this->objects.begin(); it != this->objects.end(); it++)
    {
      if (*it == object)
      {
        this->deselectObject();
        this->objects.erase(it);
        break;
      }
    }
  }

  /**
   * @brief Rasteriza a cena utilizando o pipeline descrito por Adair
   *
   * @note A rasterização da cena é responsável por renderizar todos os objetos
   * da cena na tela
   */
  void Scene::rasterize_adair_pipeline()
  {
    // for (int i = 0; i < this->omni_lights.size(); i++)
    //   LightOrbital(&this->omni_lights[i], 0.02f);

    models::Camera3D *camera = this->getCamera();

    // Obtém as matrizes de transformação
    core::Matrix sru_src_matrix = math::pipeline_adair::sru_to_src(camera->position, camera->target);
    core::Matrix projection_matrix = math::pipeline_adair::projection(
        camera->position,
        camera->target,
        camera->d);
    core::Matrix viewport_matrix = math::pipeline_adair::src_to_srt(
        this->getMinWindow(),
        this->getMinViewport(),
        this->getMaxWindow(),
        this->getMaxViewport(),
        true);

    // Multiplica as matrizes
    core::Matrix result = math::MatrixMultiply(viewport_matrix, projection_matrix);
    result = math::MatrixMultiply(result, sru_src_matrix);

    core::Vector4 vectorResult = {0.0f, 0.0f, 0.0f, 0.0f};

    // Rasteriza todos os objetos da cena
    for (auto object : this->objects)
    {
      for (auto vertex : object->getVertices())
      {
        vectorResult = math::MatrixMultiplyVector(result, vertex->getVector());

        vertex->setVectorScreen({vectorResult.x / vectorResult.w, vectorResult.y / vectorResult.w, vectorResult.z});
      }

      for (auto face : object->getFaces())
      {
        face->setVisible(face->isVisible(camera->position));
      }

      object->determineNormals();
    }

    // Remove de cena os objetos completamente fora da viewport
    for (auto object : this->objects)
    {
      if (object->isOutsideViewport(this->getMinViewport(), this->getMaxViewport()))
      {
        this->removeObject(object);
      }
    }

    // Rasteriza a luz
    core::Vector3 Light_position = this->omni_lights[0].position;
    core::Vector4 light = math::MatrixMultiplyVector(result, {Light_position.x, Light_position.y, Light_position.z, 1.0f});

    this->omni_lights[0].screen_position = {light.x / light.w, light.y / light.w, light.z};
  }

  /**
   * @brief Rasteriza a cena utilizando o pipeline descrito por João Madeiras Pereira
   *
   * @note A rasterização da cena é responsável por renderizar todos os objetos
   * da cena na tela
   */
  void Scene::rasterize_portugues_pipeline()
  {
    // for (int i = 0; i < this->omni_lights.size(); i++)
    //   LightOrbital(&this->omni_lights[i], 0.02f);

    models::Camera3D *camera = this->getCamera();

    // Obtém as matrizes de transformação
    core::Matrix sru_src_matrix = math::pipeline_adair::sru_to_src(camera->position, camera->target);

    float s_x = camera->d / this->max_viewport.x;
    float s_y = camera->d / this->max_viewport.y;

    core::Matrix perspective_volume_matrix = math::pipeline_portugues::perspective_volume_transformation(s_x, s_y, camera->d + 10);

    core::Matrix perspective_transformation_matrix = math::pipeline_portugues::perspective_transformation((camera->d + 10) / (camera->d - 10));

    core::Matrix projection_matrix = math::pipeline_portugues::projection(camera->d - 10, camera->d + 10);

    core::Matrix viewport_matrix = math::pipeline_adair::src_to_srt(
        this->getMinWindow(),
        this->getMinViewport(),
        this->getMaxWindow(),
        this->getMaxViewport(),
        true);

    // Multiplica as matrizes
    core::Matrix result = math::MatrixMultiply(viewport_matrix, projection_matrix);
    result = math::MatrixMultiply(result, perspective_transformation_matrix);
    result = math::MatrixMultiply(result, perspective_volume_matrix);
    result = math::MatrixMultiply(result, sru_src_matrix);

    core::Vector4 vectorResult = {0.0f, 0.0f, 0.0f, 0.0f};

    // Rasteriza todos os objetos da cena
    for (auto object : this->objects)
    {
      for (auto vertex : object->getVertices())
      {
        vectorResult = math::MatrixMultiplyVector(result, vertex->getVector());

        vertex->setVectorScreen({vectorResult.x / vectorResult.w, vectorResult.y / vectorResult.w, vectorResult.z});
        std::cout << vertex->getVectorScreen() << std::endl;

        // vertex->setVectorScreen({vectorResult.x, vectorResult.y, vectorResult.z});
        // std::cout << vectorResult << std::endl;
      }

      for (auto face : object->getFaces())
      {
        face->setVisible(face->isVisible(camera->position));
      }

      object->determineNormals();
    }
  }

  /**
   * @brief Seleciona um objeto da cena
   *
   * @param x Coordenada x do clique
   * @param y Coordenada y do clique
   *
   */
  void Scene::selectObject(int x, int y)
  {
    // iterate over all objects
    for (auto object : this->objects)
    {
      core::Vector4 box = object->getBox(true);
      if (box.x <= x && box.y <= y && box.z >= x && box.w >= y)
      {
        this->setSelectedObject(object);
        object->setSelected(true);
        return;
      }
    }
  }

  /**
   * @brief Deseleciona um objeto da cena
   *
   */
  void Scene::deselectObject()
  {
    if (this->getSelectedObject() != nullptr)
    {
      this->getSelectedObject()->setSelected(false);
      this->setSelectedObject(nullptr);
    }
  }

  /**
   * @brief Move a câmera da cena
   *
   * @param x Coordenada x do clique
   * @param y Coordenada y do clique
   */
  void Scene::moveCamera(int x, int y)
  {
  }

  void Scene::initializeBuffers()
  {
    int width = static_cast<int>(this->max_viewport.x);
    int height = static_cast<int>(this->max_viewport.y);

    // clear the buffers
    this->z_buffer.clear();
    this->color_buffer.clear();

    // Resize and initialize the z-buffer and color buffer
    this->z_buffer = std::vector<std::vector<float>>(width, std::vector<float>(height, std::numeric_limits<float>::infinity()));
    this->color_buffer = std::vector<std::vector<models::Color>>(width, std::vector<models::Color>(height, models::TRANSPARENT));
  }

  //------------------------------------------------------------------------------------------------
  // Individual Object Transformations
  //------------------------------------------------------------------------------------------------

  void Scene::translateObject(core::Vector3 translation)
  {
    if (this->selected_object == nullptr) // Nada a fazer
      return;

    core::Matrix viewportInv = math::MatrixInvert(math::pipeline_adair::src_to_srt(
        this->getMinWindow(),
        this->getMinViewport(),
        this->getMaxWindow(),
        this->getMaxViewport(),
        true));

    core::Vector3 transformedTranslation = math::Vector3Transform({translation}, viewportInv);

    for (auto vertex : this->selected_object->getVertices())
    {
      vertex->setVector({vertex->getX() + transformedTranslation.x,
                         vertex->getY() + transformedTranslation.y,
                         vertex->getZ() + transformedTranslation.z, 1.0f});
    }
  }
} // namespace models