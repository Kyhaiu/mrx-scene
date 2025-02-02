/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxuicomponents v1.0 -
 *
 *   CONVENTIONS: (Convenções)
 *
 *   IDIOM: ENGLISH
 *
 *   mrxuicomponents v1.0 -
 *
 *   CONVENTIONS:
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *   TODO:
 *
 *   LICENSE: GPL 3.0
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************/
#pragma once

#include <gui/imgui/imgui.h>

#include <utils/utils.hpp>
#include <gui/controller/controller.hpp>
#include <models/scene.hpp>
#include <models/mesh.hpp>
#include <models/camera.hpp>
#include <models/light.hpp>
#include <shapes/shapes.hpp>

namespace GUI
{
  namespace components
  {
    class HierarchyViewer
    {
    private:
      GUI::Controller *controller;
      int selected_index = -1;

    public:
      void objectRenderingNode(models::Mesh *object, int index);
      void cameraRenderingNode(models::Camera3D *camera);
      void lightRenderingNode(models::Light *light);
      void omnidirectionalLightRenderingNode(models::Omni *light, int index);

      HierarchyViewer(GUI::Controller *_controller);
      void render(models::Scene *scene);
    };
  }
} // namespace GUI