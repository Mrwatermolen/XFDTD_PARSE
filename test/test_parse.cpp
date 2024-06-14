#include <xfdtd/object/object.h>
#include <xfdtd/shape/shape.h>
#include <xfdtd_parse/material_map.h>
#include <xfdtd_parse/object_builder.h>
#include <xfdtd_parse/object_map.h>
#include <xfdtd_parse/shape_map.h>

#include <iostream>
#include <toml++/toml.hpp>

auto readShape(const auto &table) {
  auto shape_map = xfdtd::parse::ShapeMap();
  shape_map.read(*table);

  return shape_map;
}

auto readMaterial(const auto &table) {
  auto material_map = xfdtd::parse::MaterialMap();
  material_map.read(*table);

  return material_map;
}

auto printMap(const auto &map) {
  for (const auto &[key, value] : map.map()) {
    std::cout << key << "\n";
    std::cout << value->toString() << "\n";
  }
}

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

    auto shape_map = readShape(table);
    std::cout << "Shapes:\n";
    printMap(shape_map);
    std::cout << "\n";

    auto material_map = readMaterial(table);
    std::cout << "Materials:\n";
    printMap(material_map);
    std::cout << "\n";

    auto object_map = xfdtd::parse::ObjectMap();
    object_map.read(*table);
    std::cout << "Objects:\n";
    printMap(object_map);
    std::cout << "\n";

    auto object_builder = xfdtd::parse::ObjectBuilder();
    auto objects =
        object_builder.buildAllObjects(object_map, shape_map, material_map);
    for (const auto &object : objects) {
      std::cout << object->toString() << "\n";
    }

    object_map.get("ExceptionTest");

  } catch (const std::exception &e) {
    std::cerr << "Error reading file: " << file << "\n";
    std::cerr << e.what() << "\n";
    return 1;
  }
}
