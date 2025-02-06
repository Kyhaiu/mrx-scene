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
        20.0f, 15.0f, 40.0f);
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

    light.intensity = models::WHITE;

    this->global_light = light;

    models::Omni omni;
    omni.position = {2.0f, 2.0f, 2.0f};

    omni.intensity = models::ColorToChannels(models::WHITE);
    omni.id = "Omnidirectional light  " + std::to_string(this->omni_lights.size());

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
   * @todo REFATORAR ESTA FUNÇÃO E SEPARAR EM FUNÇÕES PARA CADA MODELO DE ILUMINAÇÃO
   */
  void Scene::adair_pipeline()
  {
    // for (int i = 0; i < this->omni_lights.size(); i++)
    //   LightOrbital(&this->omni_lights[i], 0.02f);

    models::Camera3D *camera = this->getCamera();

    // Pré computação da visibilidade dos objetos
    // Se o objeto estiver dentro do intervalo de near e far da câmera, ele é visível
    // Caso contrário, ele é invisível
    for (auto object : this->objects)
    {
      core::Vector3 centroid = this->centroid_algorithm == CENTROID_BY_MEAN ? object->getCentroidByMean() : object->getCentroidByWrapBox();
      core::Vector3 vrp = camera->position;
      core::Vector3 target = camera->target;

      // Direção da câmera (eixo Z do espaço da câmera)
      core::Vector3 camera_forward = math::Vector3Normalize(math::Vector3Subtract(target, vrp));
      // Vetor do centroid para a câmera
      core::Vector3 centroid_to_camera = centroid - vrp;

      // Projeção na direção da câmera (depth)
      float depth = math::Vector3DotProduct(camera_forward, centroid_to_camera);

      // Verifica se está fora do near/far
      if (depth < camera->near || depth > camera->far)
        object->is_visible = false;
      else
        object->is_visible = true;
    }

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
      if (!object->is_visible)
        continue;

      for (auto vertex : object->getVertices())
      {
        vectorResult = math::MatrixMultiplyVector(result, vertex->getVector());

        vertex->setVectorScreen({vectorResult.x / vectorResult.w, vectorResult.y / vectorResult.w, vectorResult.z});
      }

      for (auto face : object->getFaces())
      {
        face->setVisible(face->isVisible(camera->position));
      }

      // Só calcula os vetores unitários normais se o modelo de iluminação for diferente de FLAT_SHADING
      if (this->lighting_model != FLAT_SHADING)
        if (this->normal_algorithm == FOLEY_UNIT_NORMAL_VECTOR)
          object->determineNormals();
        else
          object->determineNormalsByAverage();
    }

    // Rasteriza a luz
    core::Vector3 Light_position = this->omni_lights[0].position;
    core::Vector4 light = math::MatrixMultiplyVector(result, {Light_position.x, Light_position.y, Light_position.z, 1.0f});

    this->omni_lights[0].screen_position = {light.x / light.w, light.y / light.w, light.z};

    // Rasterização dos objetos
    this->initializeBuffers();

    core::Vector2 min_viewport = this->getMinViewport();
    core::Vector2 max_viewport = this->getMaxViewport();

    for (auto object : this->getObjects())
    {
      if (!object->is_visible)
        continue;

      // Desenha a caixa envolvente do objeto se ele estiver selecionado
      if (object == this->getSelectedObject())
      {
        core::Vector4 box = object->getBox(true);
        utils::DrawBoundingBox({box.x, box.y}, {box.z, box.w}, models::YELLOW, this->z_buffer, this->color_buffer);
      }

      core::Vector3 object_centroid = {0.0f, 0.0f, 0.0f};

      if (this->centroid_algorithm == CENTROID_BY_WRAP_BOX)
        object_centroid = object->getCentroidByWrapBox();
      else if (this->centroid_algorithm == CENTROID_BY_MEAN)
        object_centroid = object->getCentroidByMean();

      for (auto face : object->getFaces())
      {

        if (!face->getVisible())
          continue;

        core::HalfEdge *he = face->getHalfEdge();

        // first = Coordenadas de tela
        // second = normal do vértice
        std::vector<std::pair<core::Vector3, core::Vector3>> vertexes;

        while (true)
        {
          vertexes.push_back(std::make_pair(he->getOrigin()->getVectorScreen(), he->getOrigin()->getNormal()));

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        // O vetor normal da face é calculado na ocultação de faces
        // precisa recortar o vetor normal do vertice também (assim simplifica o calculo de interpolação)
        // usar excel como base

        if (this->lighting_model == FLAT_SHADING)
        {
          std::vector<core::Vector3> clipped_vertexes;

          // O Flat shading precisa apenas dos vértices, sem os vetores normais
          for (auto vertex : vertexes)
            clipped_vertexes.push_back(vertex.first);

          clipped_vertexes = math::clip2D_polygon(clipped_vertexes, min_viewport, max_viewport);

          // Se o vetor de vértices for menor que 3, não é possível formar um polígono, então não é necessário desenhar
          if (clipped_vertexes.size() < 3)
            continue;

          utils::DrawFaceBufferFlatShading(clipped_vertexes, this->getCamera()->position, face->getFaceCentroid(), face->getNormal(), object->material, this->global_light, this->omni_lights, this->z_buffer, this->color_buffer);
        }
        else if (this->lighting_model == GOURAUD_SHADING)
        {
          // No Gouraud Shading, a cor de cada vértice é calculada antes do recorte

          std::vector<std::pair<core::Vector3, core::Vector3>> vertexes_gouraud;

          core::Vector3 eye = this->getCamera()->position;
          models::Material object_material = object->material;

          for (auto vertex : vertexes)
          {
            core::Vector3 v = vertex.first;
            core::Vector3 n = vertex.second;
            models::Color c = models::GouraudShading(this->global_light, this->omni_lights, std::make_pair(v, n), eye, object_material);
            core::Vector3 color = {static_cast<float>(c.r), static_cast<float>(c.g), static_cast<float>(c.b)};
            vertexes_gouraud.push_back(std::make_pair(v, color));
          }

          vertexes_gouraud = math::clip2D_polygon(vertexes_gouraud, min_viewport, max_viewport);

          std::vector<std::pair<core::Vector3, models::Color>> clipped_vertexes;

          for (auto vertex : vertexes_gouraud)
          {
            clipped_vertexes.push_back(std::make_pair(vertex.first, models::ChannelsToColor({vertex.second.x, vertex.second.y, vertex.second.z})));
          }

          // Se o vetor de vértices for menor que 3, não é possível formar um polígono, então não é necessário desenhar
          if (vertexes_gouraud.size() < 3)
            continue;

          utils::DrawFaceBufferGouraudShading(clipped_vertexes, this->z_buffer, this->color_buffer);
        }
        else if (this->lighting_model == PHONG_SHADING)
        {
          std::vector<std::pair<core::Vector3, core::Vector3>> clipped_vertex = math::clip2D_polygon(vertexes, min_viewport, max_viewport);

          // Se o vetor de vértices for menor que 3, não é possível formar um polígono, então não é necessário desenhar
          if (clipped_vertex.size() < 3)
            continue;

          utils::DrawFaceBufferPhongShading(clipped_vertex, object_centroid, this->getCamera()->position, object->material, this->global_light, this->omni_lights, this->z_buffer, this->color_buffer);
        }
      }
    }

    // Resetar a clipping flag de cada vértice para a próxima iteração
    for (auto object : this->getObjects())
      object->is_visible = true;
  }

  /**
   * @brief Função para rasterizar a cena utilizando o pipeline de Smith
   *
   * @todo REFATORAR ESTA FUNÇÃO E SEPARAR EM FUNÇÕES PARA CADA MODELO DE ILUMINAÇÃO
   */
  void Scene::smith_pipeline()
  {
    models::Camera3D *camera = this->getCamera();

    models::LightOrbital(&this->omni_lights[0], 0.02f);

    // Esta etapa é idêntica ao pipeline de Adair
    core::Matrix sru_src_matrix = math::pipeline_adair::sru_to_src(camera->position, camera->target, LEFT_HANDED);

    core::Vector2 window_size = this->getMaxWindow();

    window_size.x = window_size.x - this->getMinWindow().x;
    window_size.y = window_size.y - this->getMinWindow().y;

    core::Matrix clipping_transformation_matrix = math::pipeline_smith::clipping_transformation(camera->d, camera->far, core::Vector2{0.0f, 0.0f}, window_size);
    core::Matrix perspective_transformation_matrix = math::pipeline_smith::perspective_transformation(camera->near, camera->far);
    // Esta etapa é idêntica ao pipeline de Adair
    // core::Matrix viewport_matrix = math::pipeline_adair::src_to_srt(
    //     this->getMinWindow(),
    //     this->getMinViewport(),
    //     this->getMaxWindow(),
    //     this->getMaxViewport(),
    //     true);
    core::Matrix viewport_matrix = math::pipeline_smith::src_to_srt(
        this->getMinViewport(),
        this->getMaxViewport(),
        this->getCamera()->near,
        this->getCamera()->far);

    // core::Matrix result = math::MatrixMultiply(viewport_matrix, perspective_transformation_matrix);
    core::Matrix result = math::MatrixMultiply(perspective_transformation_matrix, clipping_transformation_matrix);
    result = math::MatrixMultiply(result, sru_src_matrix);

    // first = Vértice no SRC (sistema de câmera)
    // second = normal/cor do vértice (usado no gouraud/phong)
    std::vector<std::pair<core::Vector4, core::Vector3>> clipped_vertices;

    core::Vector4 vectorResult = {0.0f, 0.0f, 0.0f, 0.0f};

    // Rasterização dos objetos
    this->initializeBuffers();

    for (auto object : this->getObjects())
    {
      // Só calcula os vetores unitários normais se o modelo de iluminação for diferente de FLAT_SHADING
      if (this->lighting_model != FLAT_SHADING)
        if (this->normal_algorithm == FOLEY_UNIT_NORMAL_VECTOR)
          object->determineNormals();
        else
          object->determineNormalsByAverage();

      core::Vector3 centroid = this->centroid_algorithm == CENTROID_BY_MEAN ? object->getCentroidByMean() : object->getCentroidByWrapBox();

      // usado como variável temporária pro flat shading e pro bounding box
      std::vector<core::Vector3> vertexes_object;
      for (auto face : object->getFaces())
      {

        core::HalfEdge *he = face->getHalfEdge();
        clipped_vertices.clear();

        while (true)
        {
          core::Vector4 v = he->getOrigin()->getVector();

          vectorResult = math::MatrixMultiplyVector(result, v);

          vectorResult.x = vectorResult.x;
          vectorResult.y = vectorResult.y;
          vectorResult.z = vectorResult.z;
          clipped_vertices.push_back(std::make_pair(vectorResult, he->getOrigin()->getNormal()));

          he = he->getNext();
          if (he == face->getHalfEdge())
            break;
        }

        if (this->lighting_model == GOURAUD_SHADING)
        {
          // No Gouraud Shading, a cor de cada vértice é calculada antes do recorte

          core::Vector3 eye = this->getCamera()->position;
          models::Material object_material = object->material;

          for (auto &vertex : clipped_vertices)
          {
            core::Vector4 v = vertex.first;
            core::Vector3 n = vertex.second;
            models::Color c = models::GouraudShading(this->global_light, this->omni_lights, std::make_pair(v.toVector3(), n), eye, object_material);
            core::Vector3 color = {static_cast<float>(c.r), static_cast<float>(c.g), static_cast<float>(c.b)};
            vertex = std::make_pair(v, color);
          }
        }

        if (this->clipping)
          clipped_vertices = math::clip3D_polygon(clipped_vertices);

        for (auto &v : clipped_vertices)
        {
          core::Vector4 v_screen = math::MatrixMultiplyVector(viewport_matrix, v.first);

          v_screen.x = v_screen.x / v_screen.w;
          v_screen.y = v_screen.y / v_screen.w;
          v_screen.z = v_screen.z / v_screen.w;

          v.first = v_screen;
        }

        face->setVisible(face->isVisible(camera->position));

        if (!face->getVisible())
          continue;

        if (this->lighting_model == FLAT_SHADING)
        {
          if (clipped_vertices.size() < 3)
            continue;

          // usado como variável temporária pro flat shading e pro bounding box
          std::vector<core::Vector3> vertexes;

          for (auto vertex : clipped_vertices)
          {
            vertexes.push_back({vertex.first.x, vertex.first.y, vertex.first.z});
            vertexes_object.push_back({vertex.first.x, vertex.first.y, vertex.first.z});
          }

          utils::DrawFaceBufferFlatShading(vertexes, this->getCamera()->position, face->getFaceCentroid(), face->getNormal(), object->material, this->global_light, this->omni_lights, this->z_buffer, this->color_buffer);
        }
        else if (this->lighting_model == GOURAUD_SHADING)
        {

          std::vector<std::pair<core::Vector3, models::Color>> clipped_gouraud_vertexes;

          for (auto vertex : clipped_vertices)
          {
            clipped_gouraud_vertexes.push_back(std::make_pair(vertex.first.toVector3(), models::ChannelsToColor({vertex.second.x, vertex.second.y, vertex.second.z})));
            vertexes_object.push_back({vertex.first.x, vertex.first.y, vertex.first.z});
          }
          // Se o vetor de vértices for menor que 3, não é possível formar um polígono, então não é necessário desenhar
          if (clipped_gouraud_vertexes.size() < 3)
            continue;

          utils::DrawFaceBufferGouraudShading(clipped_gouraud_vertexes, this->z_buffer, this->color_buffer);
        }
        else if (this->lighting_model == PHONG_SHADING)
        {

          // Se o vetor de vértices for menor que 3, não é possível formar um polígono, então não é necessário desenhar
          if (clipped_vertices.size() < 3)
            continue;

          std::vector<std::pair<core::Vector3, core::Vector3>> vertexes;

          for (auto v : clipped_vertices)
          {
            vertexes.push_back({v.first.toVector3(), v.second});
            vertexes_object.push_back({v.first.x, v.first.y, v.first.z});
          }

          utils::DrawFaceBufferPhongShading(vertexes, centroid, this->getCamera()->position, object->material, this->global_light, this->omni_lights, this->z_buffer, this->color_buffer);
        }
      }

      if (object == this->getSelectedObject())
      {
        core::Vector4 box = utils::GetBoundingBox(vertexes_object);
        utils::DrawBoundingBox({box.x, box.y}, {box.z, box.w}, models::YELLOW, this->z_buffer, this->color_buffer);
      }
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
    int width = static_cast<int>(this->max_viewport.x + 1);
    int height = static_cast<int>(this->max_viewport.y + 1);

    // clear the buffers
    this->z_buffer.clear();
    this->color_buffer.clear();

    // Resize and initialize the z-buffer and color buffer
    this->z_buffer = std::vector<std::vector<float>>(width, std::vector<float>(height, std::numeric_limits<float>::infinity()));
    this->color_buffer = std::vector<std::vector<models::Color>>(width, std::vector<models::Color>(height, models::TRANSPARENT));
  }

  /**
   * @brief Função para converter a cena em um objeto json
   *
   * @return json
   */
  json Scene::to_json()
  {
    json j;

    j["camera"] = this->camera->to_json();

    j["objects"] = json::array();
    for (auto object : this->objects)
    {
      j["objects"].push_back(object->to_json());
    }

    j["min_viewport"] = this->min_viewport.to_json();
    j["max_viewport"] = this->max_viewport.to_json();
    j["min_window"] = this->min_window.to_json();
    j["max_window"] = this->max_window.to_json();
    j["illumination"] = this->lighting_model;
    j["pipeline"] = this->pipeline_model;

    return j;
  }

  /**
   * @brief Função para converter um objeto json em uma cena
   *
   * @param json_data json
   */
  void Scene::from_json(json json_data)
  {
    this->camera = this->camera->from_json(json_data["camera"]);

    this->objects.clear();

    for (auto object : json_data["objects"])
    {
      models::Mesh *mesh = new models::Mesh();
      mesh->from_json(object);
      this->objects.push_back(mesh);
    }

    this->min_viewport = core::Vector2::from_json(json_data["min_viewport"]);
    this->max_viewport = core::Vector2::from_json(json_data["max_viewport"]);
    this->min_window = core::Vector2::from_json(json_data["min_window"]);
    this->max_window = core::Vector2::from_json(json_data["max_window"]);

    this->lighting_model = json_data["illumination"];
    this->pipeline_model = json_data["pipeline"];
  }

  //------------------------------------------------------------------------------------------------
  // Individual Object Transformations
  //------------------------------------------------------------------------------------------------

  /**
   * @brief Função que translada o objeto selecionado da cena
   *
   * @param translation Deslocamento a ser aplicado ao objeto
   */
  void Scene::translateObject(core::Vector3 translation)
  {
    if (this->selected_object == nullptr) // Nada a fazer
      return;

    // Inverte a matriz de viewport
    // Obs.: A inversão é necessária pois estamos trabalhando com coordenadas de tela (SRT)
    // e a transformação deve ser feita em relação ao espaço do objeto (SRU)
    // NOTA: NO PIPELINE DO ADAIR, NÃO É POSSÍVEL REVETER AS COORDENADAS DE TELA PARA O SRU, POIS A PROJEÇÃO NÃO É INVERTÍVEL
    // LOGO É USADO APENAS A INVERSÃO DA MATRIZ DE VIEWPORT (APROXIMAÇÃO)
    core::Matrix viewportInv = math::MatrixInvert(math::pipeline_adair::src_to_srt(
        this->getMinWindow(),
        this->getMinViewport(),
        this->getMaxWindow(),
        this->getMaxViewport(),
        true));

    core::Vector3 transformedTranslation;

    transformedTranslation = math::Vector3Transform({translation}, viewportInv);

    for (auto vertex : this->selected_object->getVertices())
    {
      vertex->setVector({vertex->getX() + transformedTranslation.x,
                         vertex->getY() + transformedTranslation.y,
                         vertex->getZ() + transformedTranslation.z, 1.0f});
    }
  }

  /**
   * @brief Função que rotaciona o objeto selecionado da cena
   *
   * @param axis Eixo de rotação
   * @param angle Ângulo de rotação
   */
  void Scene::rotateObject(core::Vector3 axis, float angle)
  {
    if (this->selected_object == nullptr) // Nada a fazer
      return;

    core::Matrix rotateMatrix = math::MatrixRotate(axis, angle);

    for (auto vertex : this->selected_object->getVertices())
    {
      core::Vector4 vector = vertex->getVector();
      core::Vector4 result = math::MatrixMultiplyVector(rotateMatrix, vector);

      vertex->setVector(result);
    }
  }

  void Scene::scaleObject(core::Vector3 scale)
  {
    if (this->selected_object == nullptr) // Nada a fazer
      return;

    core::Matrix scaleMatrix = math::MatrixScale(scale);

    for (auto vertex : this->selected_object->getVertices())
    {
      core::Vector4 vector = vertex->getVector();
      core::Vector4 result = math::MatrixMultiplyVector(scaleMatrix, vector);

      vertex->setVector(result);
    }
  }

  /**
   * @brief Função que rotaciona o objeto selecionado da cena
   *
   * @param angle Ângulo de rotação em torno dos eixos x, y e z
   *
   * @note Os ângulos são fornecidos em radianos
   */
  void Scene::rotateObject(core::Vector3 angle)
  {
    if (this->selected_object == nullptr) // Nada a fazer
      return;

    core::Matrix rotateMatrix = math::MatrixRotateXYZ(angle);

    for (auto vertex : this->selected_object->getVertices())
    {
      core::Vector4 vector = vertex->getVector();
      core::Vector4 result = math::MatrixMultiplyVector(rotateMatrix, vector);

      vertex->setVector(result);
    }
  }
} // namespace models