#include <gui/imgui/imgui.h>

#include <gui/imgui-sdl2/imgui_impl_sdl2.h>
#include <gui/imgui-sdl2/imgui_impl_sdlrenderer2.h>
#include <SDL.h>

#include <iostream>
#include <core/vertex.hpp>

#include <models/camera.hpp>
#include <models/scene.hpp>

#include <math/pipeline.hpp>
#include <math/math.hpp>

#include <utils/utils.hpp>

#include <gui/view/view.hpp>
#include <gui/controller/controller.hpp>

#define SDL_main main

int main(int argc, char *argv[])
{

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Get the window size SDL2
  SDL_DisplayMode dm;

  SDL_GetCurrentDisplayMode(0, &dm);
  int window_width = dm.w;
  int window_height = dm.h;
  std::cout << "window size: " << window_width << "x" << window_height << std::endl;

  // Setup window
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
  SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+SDLRenderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, window_flags);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  // Inicialize o ImGui com SDL
  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  GUI::UI *ui = new GUI::UI(window, renderer);

  // Main loop
  bool done = false;
  while (!done)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ui->handleEvents(event, ui->window, ui->renderer);
    }

    ui->render();
  }

  // Cleanup
  delete ui;
  return 0;
}

// #include <SFML/Graphics.hpp>
// #include <gui/imgui/imgui.h>

// #include <vector>
// #include <core/vector.hpp>
// #include <limits>
// #include <algorithm>

// int main()
// {
//   core::Vector2 window_size = {1920, 1080};

//   core::Vector3 A = {400, 500, 0};
//   core::Vector3 C = {900, 500, 0};
//   core::Vector3 B = {900, 900, 0};
//   core::Vector3 D = {500, 900, 0};

//   std::vector<core::Vector3> vertexes = {A, C, C, B, B, D, D, A};

//   std::vector<std::vector<core::Vector3>> buffer(1920, std::vector<core::Vector3>(1080, {0, 0, 0}));
//   std::vector<std::vector<sf::Color>> color_buffer(1920, std::vector<sf::Color>(1080, sf::Color::Black));

//   int y_min = std::numeric_limits<int>::max();
//   int y_max = std::numeric_limits<int>::min();

//   for (auto vertex : vertexes)
//   {
//     if (vertex.y < y_min)
//       y_min = static_cast<int>(vertex.y);
//     if (vertex.y > y_max)
//       y_max = static_cast<int>(vertex.y);
//   }

//   std::vector<std::vector<core::Vector3>> scanlines(static_cast<int>(y_max) - static_cast<int>(y_min) + 1);

//   for (int i = 0; i < vertexes.size(); i = i + 2)
//   {
//     core::Vector3 start = vertexes[i];
//     core::Vector3 end = vertexes[i + 1];

//     if (start.y == end.y)
//       continue;

//     if (start.y > end.y)
//     {
//       std::swap(start, end);
//     }

//     float m = (end.x - start.x) / (end.y - start.y); // Inclinação da linha (dx/dy)
//     float x = start.x;                               // Use um float para evitar erros de arredondamento
//     for (int y = static_cast<int>(start.y); y <= static_cast<int>(end.y); y++)
//     {
//       scanlines[y - y_min].push_back({x, static_cast<float>(y), 0.0f});
//       x += m; // Incrementa x com base na inclinação m
//     }
//   }

//   sf::RenderWindow window(sf::VideoMode(static_cast<int>(window_size.x), static_cast<int>(static_cast<int>(window_size.y))), "Color Buffer Display");

//   ImGui::SFML::Init(window); // Inicializa o ImGui com SFML

//   while (window.isOpen())
//   {
//     sf::Event event;
//     while (window.pollEvent(event))
//     {
//       if (event.type == sf::Event::Closed)
//         window.close();

//       ImGui::SFML::ProcessEvent(event); // Processa eventos do ImGui
//     }

//     // Processa os scanlines
//     for (auto scanline : scanlines)
//     {
//       std::sort(scanline.begin(), scanline.end(), [](core::Vector3 a, core::Vector3 b)
//                 { return a.x < b.x; });

//       for (int i = 0; i < scanline.size() - 1; i = i + 2)
//       {
//         core::Vector3 start = scanline[i];
//         core::Vector3 end = scanline[i + 1];

//         for (int x = static_cast<int>(start.x); x < static_cast<int>(end.x); x++)
//         {
//           buffer[x][static_cast<int>(start.y)] = {static_cast<float>(x), static_cast<float>(start.y), 0.0f};
//           color_buffer[x][static_cast<int>(start.y)] = sf::Color::White;
//         }
//       }
//     }

//     ImGui::SFML::Update(window, sf::seconds(1.f / 60.f)); // Atualiza o ImGui

//     sf::Sprite sprite;

//     sf::Texture texture;
//     texture.create(static_cast<int>(window_size.x), static_cast<int>(window_size.y));

//     sf::Image image;
//     image.create(static_cast<int>(window_size.x), static_cast<int>(window_size.y), sf::Color::Black);

//     for (int i = 0; i < static_cast<int>(window_size.x); i++)
//     {
//       for (int j = 0; j < static_cast<int>(window_size.y); j++)
//       {
//         image.setPixel(i, j, color_buffer[i][j]);
//       }
//     }

//     texture.update(image);
//     sprite.setTexture(texture);

//     window.clear();
//     ImGui::SFML::Render(window); // Renderiza o ImGui
//     window.draw(sprite);
//     window.display();
//   }

//   ImGui::SFML::Shutdown(); // Finaliza o ImGui

//   return 0;
// }
