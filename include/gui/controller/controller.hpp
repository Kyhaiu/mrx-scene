#pragma once

#include <gui/imgui-sdl2/imgui_impl_sdl2.h>
#include <gui/imgui-sdl2/imgui_impl_sdlrenderer2.h>
#include <SDL.h>

#include <models/scene.hpp>
#include <models/camera.hpp>
#include <utils/file.hpp>

namespace GUI
{

  /**
   * @brief Estrutura que representa a posição do mouse
   *
   */
  typedef struct
  {
    int x;
    int y;
  } mouse_position;

  /**
   * @brief Estrutura que contém os parâmetros de inserção de objetos
   *
   * @param subdivisions int - Número de subdivisões (para esferas)
   * @param radius float - Raio (para esferas e cones)
   * @param height  float - Altura (para cones)
   * @param segments int - Segmentos (para cilindros)
   * @param rings int - Anéis (para esferas)
   *
   * @note Utilizado no menu de inserção de objetos
   */
  typedef struct
  {
    int subdivisions;
    float radius;
    float height;
    int segments;
    int rings;
  } insertion_options_parameters;

  class Controller
  {
  private:
    /**
     * @brief Ponteiro para o modelo da cena 3D
     */
    models::Scene *scene;

  public:
    // Properties
    /**
     * @brief Largura da janela
     *
     */
    int windowWidth;
    /**
     * @brief Altura da janela
     *
     */
    int windowHeight;
    /**
     * @brief Controla a visibilidade da barra lateral
     */
    bool showSidebar = false;
    /**
     * @brief Controla a visibilidade do menu de inserção de objetos
     */
    bool showInsertOptions = false;
    /**
     * @brief Posição atual do mouse
     */
    mouse_position mousePosition;
    /**
     * @brief Posição do mouse no frame anterior
     */
    mouse_position lastMousePosition;
    /**
     * @brief Flag que indica se o mouse está pressionado
     */
    bool isMouseHeld = false;
    /**
     * @brief Flag que indica se a câmera está selecionada
     */
    bool camera_is_selected = false;
    /**
     * @brief Sensibilidade da rotação da camera
     */
    float camera_rotation_sensitivity = 0.01667f;
    /**
     * @brief Parâmetros de inserção de objetos
     */
    insertion_options_parameters insertionOptions;

    // Constructor and Destructor
    Controller(float canvasWidth, float canvasHeight);
    ~Controller();

    // Getters and Setters
    models::Scene *getScene();
    void setScene(models::Scene &_scene);

    // Methods
    void updateScene();
    void addObject(models::Mesh *object);
    void removeObject(models::Mesh *object);
    void selectObject(models::Mesh *object);
    void newScene();

    // Events
    void on_hierarchy_item_selected(int index);
    void handleEvents(const SDL_Event &event, SDL_Window *window, float deltaTime);
    void on_file_dialog_open(const std::string &file);
    void save_scene();
  };
}