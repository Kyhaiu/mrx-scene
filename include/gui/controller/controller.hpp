#pragma once

#include <gui/imgui-sdl2/imgui_impl_sdl2.h>
#include <gui/imgui-sdl2/imgui_impl_sdlrenderer2.h>
#include <SDL.h>

#include <models/scene.hpp>
#include <models/camera.hpp>
#include <models/benchmark.hpp>
#include <utils/file.hpp>
#include <filesystem> // Para verificação de diretório

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
    float inner_radius;
    float outer_radius;
    int major_segments;
    int minor_segments;
  } insertion_options_parameters;

  enum ItemSelected
  {
    NONE,
    OBJECT,
    GLOBAL_LIGHT,
    OMNI_LIGHT,
    CAMERA
  };

#define FREE_MOVEMENT 0
#define ORBITAL_MOVEMENT 1

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
     * @brief Flag usada para indicar se o benchmarking está ativo
     */
    bool benchmarking = false;
    /**
     * @brief Sensibilidade da rotação da camera
     */
    float camera_rotation_sensitivity = 0.01667f;
    /**
     * @brief Tipo de movimentação utilizada na câmera
     *
     * @note FREE_MOVEMENT = 0 (Movimentação livre WASD)
     * @note ORBITAL_MOVEMENT = 1 (Movimento Orbital em torno do foco)
     */
    int type_camera_movement = FREE_MOVEMENT;
    /**
     * @brief Angulo acumulado da camera
     *
     * @note Usado no benchmark para trocar a movimentação da câmera
     */
    float accumulatedAngle = 0.0f;
    /**
     * @brief Parâmetros de inserção de objetos
     */
    insertion_options_parameters insertionOptions;
    /**
     * @brief Mapa para armazenar translações anteriores por objeto
     *
     * @note TODO: BUSCAR NÃO DEPENDER DE DOIS ESTADOS PARA REPRESENTAR A MESMA COISA
     */
    std::map<models::Mesh *, core::Vector3> previousTranslations;
    /**
     * @brief Mapa para armazenar rotações anteriores por objeto
     *
     * @note TODO: BUSCAR NÃO DEPENDER DE DOIS ESTADOS PARA REPRESENTAR A MESMA COISA
     */
    std::map<models::Mesh *, core::Vector3> previousRotation;
    /**
     * @brief Mapa para armazenar escalas anteriores por objeto
     *
     * @note TODO: BUSCAR NÃO DEPENDER DE DOIS ESTADOS PARA REPRESENTAR A MESMA COISA
     */
    std::map<models::Mesh *, core::Vector3> previousScale;
    /**
     * @brief Resultados do benchmarking
     *
     */
    models::Benchmark benchmark_results;
    /**
     * @brief Quantidade de repetições do benchmarking
     *
     */
    int benchmark_repetitions = 1;
    /**
     * @brief Tipo do elemento selecionado na cena
     *
     * @note NONE - 0, OBJECT - 2, LIGHT - 3, CAMERA - 4
     */
    int element_selected_type = ItemSelected::NONE;
    /**
     * @brief Indice do elemento selecionado
     *
     * @note -1 para nenhum elemento selecionado
     * @note -2 para câmera
     * @note -3 para luz global
     */
    int selected_element_index = -1;

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
    void translate_object(models::Mesh *object, core::Vector3 translation);
    void rotate_object(models::Mesh *object, core::Vector3 rotation);
    void scale_object(models::Mesh *object, core::Vector3 scale);

    // Events
    void on_hierarchy_item_selected(int item_type, int object_index);
    void handleEvents(const SDL_Event &event, SDL_Window *window, float deltaTime);
    void on_file_dialog_open(const std::string &file);
    void save_scene();
    void start_benchmark();
    void end_benchmark();
    void update_benchmark(double frame_time);
    void update_camera_benchmark();
  };
}