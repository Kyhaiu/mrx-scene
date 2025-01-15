/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrximgui_sfml_utils v1.0 - Conjunto de funções utilitárias para trabalhar com ImGui e SFML neste projeto
 *
 *   CONVENTIONS: (Convenções)
 *
 *   IDIOM: ENGLISH
 *
 *   mrximgui_sfml_utils v1.0 - Set of utility functions to work with ImGui and SFML in this project
 *
 *   CONVENTIONS:

 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
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

#include <fstream>
#include <iostream>

#include <utils/nlohmann/json.hpp>

using json = nlohmann::json;

namespace utils
{

  json load_json(const std::string &file_path);
  void save_json(const std::string &file_path, const json &json_data);
} // namespace utils