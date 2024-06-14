#include <xfdtd_parse/exception.h>
#include <xfdtd_parse/material_map.h>
#include <xfdtd_parse/object_map.h>
#include <xfdtd_parse/shape_map.h>

#include <string>

namespace xfdtd {

namespace parse {

auto ObjectEntry::toString() const -> std::string {
  std::stringstream ss;
  ss << "ObjectEntry: {name: " << _name << ", shape: " << _shape_name
     << ", material: " << _material_name << ", priority: " << _priority << "}";
  return ss.str();
}

auto ObjectMap::read(const toml::table& table) -> void {
  auto object_arr = table.get_as<toml::array>("object");
  if (!object_arr) {
    throw XFDTDParseObjectMapException("ObjectMap::read: Missing field object");
  }

  for (const auto& object : *object_arr) {
    auto object_table = object.as_table();
    if (!object_table) {
      throw XFDTDParseObjectMapException(
          "ObjectMap::read: Object is not a table");
    }

    try {
      auto [name, o] = makeObjectEntry(*object_table);
      addObjectEntry(name, std::move(o));
    } catch (const XFDTDParseObjectMapException& e) {
      std::stringstream ss;
      ss << "Warning: occurred exception in ObjectMap::read: " << e.what()
         << ". Skip this object.\n";
      std::cerr << ss.str();
    }
  }
}

auto ObjectMap::getObjectNameSortedByPriority() const
    -> std::vector<std::string> {
  std::vector<std::string> names;
  for (const auto& [name, object] : _objects) {
    names.push_back(name);
  }

  std::sort(names.begin(), names.end(),
            [this](const std::string& a, const std::string& b) {
              return _objects.at(a)->priority() < _objects.at(b)->priority();
            });

  return names;
}

auto ObjectMap::makeObjectEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<ObjectEntry>> {
  auto name = table["name"].value<std::string>();
  if (!name) {
    std::stringstream ss;
    ss << "ObjectMap::makeObjectRecord: Missing field name";
    throw XFDTDParseObjectRecordException(ss.str());
  }

  auto shape_name = table["shape"].value<std::string>();
  if (!shape_name) {
    std::stringstream ss;
    ss << "ObjectMap::makeObjectRecord: Missing field shape";
    throw XFDTDParseObjectRecordException(ss.str());
  }

  auto material_name = table["material"].value<std::string>();
  if (!material_name) {
    std::stringstream ss;
    ss << "ObjectMap::makeObjectRecord: Missing field material";
    throw XFDTDParseObjectRecordException(ss.str());
  }

  auto priority = table["priority"].value<int>();
  if (!priority) {
    std::stringstream ss;
    ss << "ObjectMap::makeObjectRecord: Missing field priority";
    throw XFDTDParseObjectRecordException(ss.str());
  }

  return {name.value(), std::make_unique<ObjectEntry>(
                            name.value(), shape_name.value(),
                            material_name.value(), priority.value())};
}

auto ObjectMap::addObjectEntry(const std::string& name,
                               std::unique_ptr<ObjectEntry> object) -> void {
  if (require_unique_name && _objects.count(name) != 0) {
    std::stringstream ss;
    ss << "ObjectMap::addObjectEntry: Duplicate object name: " << name;
    throw XFDTDParseObjectMapException(ss.str());
  }

  _objects[name] = std::move(object);
}

}  // namespace parse

}  // namespace xfdtd