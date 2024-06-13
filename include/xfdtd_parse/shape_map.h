#ifndef __XFDTD_PARSE_SHAPE_MAP_H__
#define __XFDTD_PARSE_SHAPE_MAP_H__

#include <xfdtd/shape/shape.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <toml++/toml.hpp>
#include <tuple>
#include <unordered_map>

#include "xfdtd_parse/exception.h"

namespace xfdtd {

namespace parse {

class XFDTDParseShapeMapException : public XFDTDParseException {
 public:
  XFDTDParseShapeMapException(const std::string& message)
      : XFDTDParseException(message) {}
};

class ShapeMap {
 public:
  enum class ShapeType {
    Cube,
    Cylinder,
    Sphere,
  };

  static constexpr auto shapeTypeToKey(ShapeType type) -> std::string;

 public:
  ShapeMap();

  auto read(const toml::table& table) -> void;

  auto map() const
      -> const std::unordered_map<std::string, std::unique_ptr<Shape>>& {
    return _shapes;
  }

 private:
  template <ShapeType S>
  auto readShape(const toml::table& table) -> void;

  template <ShapeType S>
  auto makeShape(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Shape>>;

  auto addShape(const std::string& name, std::unique_ptr<Shape> shape) -> void;

  auto makeCube(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Shape>>;

  auto makeCylinder(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Shape>>;

  auto makeSphere(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Shape>>;

  std::unordered_map<std::string, std::unique_ptr<Shape>> _shapes;
};

inline constexpr auto ShapeMap::shapeTypeToKey(ShapeType type) -> std::string {
  switch (type) {
    case ShapeType::Cube:
      return "cube";
    case ShapeType::Cylinder:
      return "cylinder";
    case ShapeType::Sphere:
      return "sphere";
  }
}

template <ShapeMap::ShapeType S>
auto ShapeMap::readShape(const toml::table& table) -> void {
  if constexpr (S == ShapeType::Cylinder) {
    throw XFDTDParseShapeMapException(
        "ShapeMap::readShape: Not implemented Cylinder");
  }

  constexpr auto key = shapeTypeToKey(S);
  auto arr = table.get_as<toml::array>(key);

  if (!arr) {
    std::stringstream ss;
    ss << "Error: ShapeMap::readShape Can't find key: " << key;
    throw XFDTDParseShapeMapException(ss.str());
  }

  for (const auto& t : *arr) {
    if (t.is_table() == false) {
      std::stringstream ss;
      ss << "Error: readShape: Invalid table";
      throw XFDTDParseShapeMapException(ss.str());
    }

    try {
      auto [name, shape] = makeShape<S>(*t.as_table());
      addShape(name, std::move(shape));
    } catch (const XFDTDParseShapeMapException& e) {
      std::stringstream ss;
      ss << "Waring: occurred exception in readShape: " << e.what()
         << ". Skip this shape.\n";
      std::cout << ss.str();
    }
  }
}

template <ShapeMap::ShapeType S>
auto ShapeMap::makeShape(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<Shape>> {
  switch (S) {
    case ShapeType::Cube:
      return makeCube(table);
    case ShapeType::Cylinder:
      return makeCylinder(table);
    case ShapeType::Sphere:
      return makeSphere(table);
  }
}

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_SHAPE_MAP_H__
