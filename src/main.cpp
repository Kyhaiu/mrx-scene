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

  ImFont *font = io.Fonts->AddFontFromFileTTF("./assets/fonts/Kroftsmann.ttf", 16.0f); // Fonte personalizada

  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  // Inicialize o ImGui com SDL
  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  // Recria as texturas do renderizador
  ImGui_ImplSDLRenderer2_DestroyDeviceObjects();
  ImGui_ImplSDLRenderer2_CreateDeviceObjects();

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