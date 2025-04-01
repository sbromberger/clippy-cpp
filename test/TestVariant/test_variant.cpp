

// Copyright 2021 Lawrence Livermore National Security, LLC and other CLIPPy
// Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: MIT

#include <boost/json.hpp>
#include <clippy/clippy.hpp>
#include <iostream>

static const std::string method_name = "test_variant";

using variants = std::variant<bool, int64_t, double, std::string>;

int main(int argc, char **argv) {
  clippy::clippy clip{method_name, "Tests to see whether variants are working"};
  clip.add_required<variants>("value", "A value that can be a variant");
  clip.returns<std::string>(
      "The type of the value that was passed in, as a string representation");

  // no object-state requirements in constructor
  if (clip.parse(argc, argv)) {
    return 0;
  }

  auto variant = clip.get<variants>("value");

  std::stringstream ss;
  std::visit(
      [&ss](auto &&arg) {
        try {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, std::string>) {
            ss << "string: " << arg << std::endl;

          } else if constexpr (std::is_same_v<T, int64_t>) {
            ss << "int64: " << arg << std::endl;
          } else if constexpr (std::is_same_v<T, bool>) {
            ss << "bool: " << arg << std::endl;
          } else if constexpr (std::is_same_v<T, double>) {
            ss << "double: " << arg << std::endl;
          } else {
            ss << "unknown type" << std::endl;
          }
        } catch (const std::exception &e) {
          ss << "Exception: " << e.what() << std::endl;
        }
      },
      variant);

  clip.to_return<std::string>(ss.str());
  return 0;
}
