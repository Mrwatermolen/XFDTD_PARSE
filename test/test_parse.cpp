#include <xfdtd/shape/shape.h>
#include <xfdtd_parse/shape_map.h>

#include <iostream>
#include <toml++/toml.hpp>

int main(int argc, char **argv) {
  using namespace std::string_view_literals;

  constexpr auto default_file = "config.toml"sv;

  if (argc < 2) {
    std::cout << "Reading default file: " << default_file << "\n";
  }

  auto file = argc < 2 ? default_file : argv[1];

  try {
    const auto data = toml::parse_file(file);

    auto table = data.as_table();

    auto shape_map = xfdtd::parse::ShapeMap();
    shape_map.read(*table);

    std::cout << "Shapes:\n";
    for (const auto &[key, value] : shape_map._shapes) {
      std::cout << key << "\n";
      std::cout << value->toString() << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error reading file: " << file << "\n";
    std::cerr << e.what() << "\n";
    return 1;
  }
}
