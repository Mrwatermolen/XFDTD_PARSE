#ifndef __XFDTD_PARSE_OBJECT_MAP_H__
#define __XFDTD_PARSE_OBJECT_MAP_H__

#include <xfdtd_parse/exception.h>

#include <toml++/toml.hpp>
#include <unordered_map>

namespace xfdtd {

namespace parse {

// forward declaration
class ShapeMap;
class MaterialMap;
class Object;

class XFDTDParseObjectMapException : public XFDTDParseException {
 public:
  XFDTDParseObjectMapException(const std::string& message)
      : XFDTDParseException(message) {}
};

class ObjectEntry {
 public:
  ObjectEntry() = default;

  ObjectEntry(const std::string& name, const std::string& shape_name,
              const std::string& material_name, int priority)
      : _name(name),
        _shape_name(shape_name),
        _material_name(material_name),
        _priority(priority) {}

  auto name() const -> const std::string& { return _name; }

  auto shapeName() const -> const std::string& { return _shape_name; }

  auto materialName() const -> const std::string& { return _material_name; }

  auto priority() const -> int { return _priority; }

  auto toString() const -> std::string;

 private:
  std::string _name{};
  std::string _shape_name{};
  std::string _material_name{};
  int _priority{};
};

class XFDTDParseObjectRecordException : public XFDTDParseException {
 public:
  XFDTDParseObjectRecordException(const std::string& message)
      : XFDTDParseException(message) {}
};

class ObjectMap {
 public:
  using Record = std::unordered_map<std::string, std::unique_ptr<ObjectEntry>>;

 public:
  ObjectMap() = default;

  auto read(const toml::table& table) -> void;

  auto map() const -> const Record& { return _objects; }

  auto get(const std::string& name) const -> const ObjectEntry& {
    if (_objects.find(name) == _objects.end()) {
      std::stringstream ss;
      ss << "ObjectMap::get: Object name: " << name
         << " is not found in object map";
      throw XFDTDParseObjectMapException(ss.str());
    }

    return *_objects.at(name);
  }

  auto getObjectNameSortedByPriority() const -> std::vector<std::string>;

 private:
  inline static auto require_unique_name = false;

  auto makeObjectEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<ObjectEntry>>;

  auto addObjectEntry(const std::string& name,
                      std::unique_ptr<ObjectEntry> object) -> void;

  Record _objects;
};

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_OBJECT_MAP_H__
