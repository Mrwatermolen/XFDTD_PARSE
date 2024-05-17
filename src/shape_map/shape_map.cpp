#include "xfdtd_parse/shape_map.h"

#include <xfdtd/common/type_define.h>
#include <xfdtd/shape/cube.h>
#include <xfdtd/shape/sphere.h>
#include <xfdtd_parse/shape_map.h>

#include <sstream>

#include "util.h"

namespace xfdtd::parse {

ShapeMap::ShapeMap() = default;

auto ShapeMap::read(const toml::table& table) -> void {
  auto shape_table = table.get_as<toml::table>("shape");
  readShape<ShapeType::Cube>(*shape_table);
  readShape<ShapeType::Sphere>(*shape_table);
  // readShape<ShapeType::Cylinder>(*shape_table);
}

auto ShapeMap::addShape(const std::string& name,
                        std::unique_ptr<Shape> shape) -> void {
  _shapes[name] = std::move(shape);
}

auto ShapeMap::makeCube(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<Shape>> {
  auto name = table["name"].value<std::string>();
  if (!name) {
    std::stringstream ss;
    ss << "readCube: Missing field name";
    throw XFDTDParseShapeMapException(ss.str());
  }

  auto start = table.get_as<toml::array>("start");
  if (!start) {
    std::stringstream ss;
    ss << "readCube: Missing field start in Cube: " << name.value();
    throw XFDTDParseShapeMapException(ss.str());
  }

  auto size = table.get_as<toml::array>("size");
  if (!size) {
    std::stringstream ss;
    ss << "readCube: Missing field size in Cube: " << name.value();
    throw XFDTDParseShapeMapException(ss.str());
  }

  auto start_vec = tomlArrayToVec3(*start);
  auto size_vec = tomlArrayToVec3(*size);

  return std::make_tuple(name.value(),
                         std::make_unique<Cube>(start_vec, size_vec));
}

auto ShapeMap::makeSphere(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<Shape>> {
  auto name = table["name"].value<std::string>();
  if (!name) {
    std::stringstream ss;
    ss << "readSphere: Missing field name";
    throw XFDTDParseShapeMapException(ss.str());
  }

  auto center = table.get_as<toml::array>("center");
  if (!center) {
    std::stringstream ss;
    ss << "readSphere: Missing field center in Sphere: " << name.value();
    throw XFDTDParseShapeMapException(ss.str());
  }

  std::optional<Real> radius = table["radius"].value<Real>();
  if (!radius) {
    std::stringstream ss;
    ss << "readSphere: Missing field radius in Sphere: " << name.value();
    throw XFDTDParseShapeMapException(ss.str());
  }

  auto center_vec = tomlArrayToVec3(*center);

  return std::make_tuple(name.value(), std::make_unique<Sphere>(center_vec, 1));
}

auto ShapeMap::makeCylinder(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<Shape>> {
  throw XFDTDParseShapeMapException(
      "ShapeMap::makeCylinder: Not implemented Cylinder");
}

}  // namespace xfdtd::parse