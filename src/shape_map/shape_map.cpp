#include "xfdtd_parse/shape_map.h"

#include <xfdtd/common/type_define.h>
#include <xfdtd/shape/cube.h>
#include <xfdtd/shape/sphere.h>
#include <xfdtd_parse/shape_map.h>

#include <sstream>

#include "util.h"

namespace xfdtd::parse {

CubeEntry::CubeEntry(const std::string& name, const Vector& origin,
                     const Vector& size)
    : ShapeEntry{name}, _origin{origin}, _size{size} {}

auto CubeEntry::make() const -> std::unique_ptr<Shape> {
  return std::make_unique<Cube>(_origin, _size);
}

SphereEntry::SphereEntry(const std::string& name, const Vector& center,
                         Real radius)
    : ShapeEntry{name}, _center{center}, _radius{radius} {}

auto SphereEntry::make() const -> std::unique_ptr<Shape> {
  return std::make_unique<Sphere>(_center, _radius);
}

auto ShapeMap::read(const toml::table& table) -> void {
  auto shape_table = table.get_as<toml::table>("shape");
  readShape<ShapeType::Cube>(*shape_table);
  readShape<ShapeType::Sphere>(*shape_table);
  // readShape<ShapeType::Cylinder>(*shape_table);
}

auto ShapeMap::addShapeEntry(const std::string& name,
                             std::unique_ptr<ShapeEntry> shape) -> void {
  _shapes[name] = std::move(shape);
}

auto ShapeMap::makeCubeEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<CubeEntry>> {
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

  return std::make_tuple(name.value(), std::make_unique<CubeEntry>(
                                           name.value(), start_vec, size_vec));
}

auto ShapeMap::makeSphereEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<SphereEntry>> {
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

  return std::make_tuple(
      name.value(),
      std::make_unique<SphereEntry>(name.value(), center_vec, radius.value()));
}

auto ShapeMap::makeCylinderEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<ShapeEntry>> {
  throw XFDTDParseShapeMapException(
      "ShapeMap::makeCylinder: Not implemented Cylinder");
}

}  // namespace xfdtd::parse