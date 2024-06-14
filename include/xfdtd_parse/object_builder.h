#ifndef __XFDTD_PARSE_OBJECT_BUILDER_H__
#define __XFDTD_PARSE_OBJECT_BUILDER_H__

#include <xfdtd_parse/exception.h>

#include <memory>

namespace xfdtd {

// forward declaration
class Object;

namespace parse {

// forward declaration
class MaterialMap;
class ObjectEntry;
class ObjectMap;
class ShapeMap;

class XFDTDParseObjectBuilderException : public XFDTDParseException {
 public:
  XFDTDParseObjectBuilderException(const std::string& message)
      : XFDTDParseException(message) {}
};

class ObjectBuilder {
 public:
  auto buildAllObjects(const ObjectMap& object_map, const ShapeMap& shape_map,
                       const MaterialMap& material_map) const
      -> std::vector<std::unique_ptr<xfdtd::Object>>;

  auto buildObject(const ObjectEntry& object_entry, const ShapeMap& shape_map,
                   const MaterialMap& material_map) const
      -> std::unique_ptr<xfdtd::Object>;

 private:
};

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_OBJECT_BUILDER_H__
