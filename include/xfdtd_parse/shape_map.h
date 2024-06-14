#ifndef __XFDTD_PARSE_SHAPE_MAP_H__
#define __XFDTD_PARSE_SHAPE_MAP_H__

#include <xfdtd/coordinate_system/coordinate_system.h>
#include <xfdtd/shape/shape.h>
#include <xfdtd_parse/exception.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <toml++/toml.hpp>
#include <tuple>
#include <unordered_map>

namespace xfdtd {

// forward declaration
class Shape;

namespace parse {

class ShapeEntry {
 public:
  ShapeEntry(const std::string& name) : _name{name} {}

  ShapeEntry(const ShapeEntry& other) = default;

  ShapeEntry(ShapeEntry&& other) noexcept = default;

  ShapeEntry& operator=(const ShapeEntry& other) = default;

  ShapeEntry& operator=(ShapeEntry&& other) noexcept = default;

  virtual ~ShapeEntry() = default;

  auto name() const -> const std::string& { return _name; }

  virtual auto toString() const -> std::string = 0;

  virtual auto make() const -> std::unique_ptr<Shape> = 0;

 private:
  std::string _name{};
};

class CubeEntry : public ShapeEntry {
 public:
  CubeEntry(const std::string& name, const Vector& origin, const Vector& size);

  CubeEntry(const CubeEntry& other) = default;

  CubeEntry(CubeEntry&& other) noexcept = default;

  CubeEntry& operator=(const CubeEntry& other) = default;

  CubeEntry& operator=(CubeEntry&& other) noexcept = default;

  virtual ~CubeEntry() = default;

  auto origin() const -> const Vector& { return _origin; }

  auto size() const -> const Vector& { return _size; }

  auto toString() const -> std::string override {
    std::stringstream ss;
    ss << "CubeEntry: {name: " << name() << ", origin: " << _origin.toString()
       << ", size: " << _size.toString() << "}";
    return ss.str();
  }

  auto make() const -> std::unique_ptr<Shape> override;

 private:
  Vector _origin{};
  Vector _size{};
};

class SphereEntry : public ShapeEntry {
 public:
  SphereEntry(const std::string& name, const Vector& center, Real radius);

  SphereEntry(const SphereEntry& other) = default;

  SphereEntry(SphereEntry&& other) noexcept = default;

  SphereEntry& operator=(const SphereEntry& other) = default;

  SphereEntry& operator=(SphereEntry&& other) noexcept = default;

  virtual ~SphereEntry() = default;

  auto center() const -> const Vector& { return _center; }

  auto radius() const -> Real { return _radius; }

  auto toString() const -> std::string override {
    std::stringstream ss;
    ss << "SphereEntry: {name: " << name() << ", center: " << _center.toString()
       << ", radius: " << _radius << "}";
    return ss.str();
  }

  auto make() const -> std::unique_ptr<Shape> override;

 private:
  Vector _center{};
  Real _radius{};
};

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

  using Record = std::unordered_map<std::string, std::unique_ptr<ShapeEntry>>;

 public:
  ShapeMap() = default;

  auto read(const toml::table& table) -> void;

  auto map() const -> const Record& { return _shapes; }

 private:
  template <ShapeType S>
  auto readShape(const toml::table& table) -> void;

  template <ShapeType S>
  auto makeShapeEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<ShapeEntry>>;

  auto addShapeEntry(const std::string& name,
                     std::unique_ptr<ShapeEntry> shape) -> void;

  auto makeCubeEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<CubeEntry>>;

  auto makeCylinderEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<ShapeEntry>>;

  auto makeSphereEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<SphereEntry>>;

  Record _shapes;
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
      auto [name, shape] = makeShapeEntry<S>(*t.as_table());
      addShapeEntry(name, std::move(shape));
    } catch (const XFDTDParseShapeMapException& e) {
      std::stringstream ss;
      ss << "Waring: occurred exception in readShape: " << e.what()
         << ". Skip this shape.\n";
      std::cout << ss.str();
    }
  }
}

template <ShapeMap::ShapeType S>
auto ShapeMap::makeShapeEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<ShapeEntry>> {
  switch (S) {
    case ShapeType::Cube:
      return makeCubeEntry(table);
    case ShapeType::Cylinder:
      return makeCylinderEntry(table);
    case ShapeType::Sphere:
      return makeSphereEntry(table);
  }
}

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_SHAPE_MAP_H__
