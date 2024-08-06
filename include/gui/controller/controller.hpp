#pragma once

#include <SFML/Graphics.hpp>

#include <models/scene.hpp>
#include <models/camera.hpp>

namespace GUI
{
  class Controller
  {
  private:
    /**
     * @brief Ponteiro para o modelo da cena 3D
     */
    models::Scene *scene;

  public:
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
    sf::Vector2i mousePosition;
    /**
     * @brief Posição do mouse no frame anterior
     */
    sf::Vector2i lastMousePosition;
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

    // Constructor and Destructor
    Controller(float canvasWidth, float canvasHeight);
    ~Controller();

    // Getters and Setters
    models::Scene *getScene();
    void setScene(models::Scene &_scene);

    // Methods
    void updateScene();
    void addObject(models::Mesh *object);

    // Events
    void on_hierarchy_item_selected(int index);
    void handleEvents(const sf::Event &event, sf::RenderWindow &window, sf::Time deltaTime);
  };
}