#include <xfdtd/object/object.h>
#include <xfdtd_parse/material_map.h>
#include <xfdtd_parse/object_builder.h>
#include <xfdtd_parse/object_map.h>
#include <xfdtd_parse/shape_map.h>

#include <sstream>

namespace xfdtd {

namespace parse {

auto ObjectBuilder::buildAllObjects(const ObjectMap& object_map,
                                    const ShapeMap& shape_map,
                                    const MaterialMap& material_map) const
    -> std::vector<std::unique_ptr<xfdtd::Object>> {
  std::vector<std::unique_ptr<xfdtd::Object>> objects;
  auto object_names = object_map.getObjectNameSortedByPriority();
  for (const auto& name : object_names) {
    auto object_entry = object_map.get(name);
    auto object = buildObject(object_entry, shape_map, material_map);
    objects.push_back(std::move(object));
  }
  return objects;
}

auto ObjectBuilder::buildObject(
    const ObjectEntry& object_entry, const ShapeMap& shape_map,
    const MaterialMap& material_map) const -> std::unique_ptr<xfdtd::Object> {
  auto shape_name = object_entry.shapeName();
  auto shape_entry = shape_map.map().find(shape_name);
  if (shape_entry == shape_map.map().end()) {
    std::stringstream ss;
    ss << "ObjectBuilder::buildObject: Shape " << shape_name
       << " is not found in shape map";
    throw XFDTDParseObjectBuilderException(ss.str());
  }

  auto material_name = object_entry.materialName();
  auto material_entry = material_map.map().find(material_name);
  if (material_entry == material_map.map().end()) {
    std::stringstream ss;
    ss << "ObjectBuilder::buildObject: Material " << material_name
       << " is not found in material map";
    throw XFDTDParseObjectBuilderException(ss.str());
  }

  return std::make_unique<xfdtd::Object>(object_entry.name(),
                                         shape_entry->second->make(),
                                         material_entry->second->make());
}

}  // namespace parse

}  // namespace xfdtd
