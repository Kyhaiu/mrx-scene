//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/utils/nlohmann/json
//
// SPDX-FileCopyrightText: 2013 - 2024 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#pragma once

#include <utils/nlohmann/detail/abi_macros.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

  // dispatching helper struct
  template <class T>
  struct identity_tag
  {
  };

} // namespace detail
NLOHMANN_JSON_NAMESPACE_END
