#ifndef __XFDTD_PARSE_UTIL_H__
#define __XFDTD_PARSE_UTIL_H__

#include <xfdtd/common/type_define.h>
#include <xfdtd/coordinate_system/coordinate_system.h>

#include <toml++/toml.hpp>

#include "xfdtd_parse/exception.h"

namespace xfdtd::parse {

inline auto tomlArrayToVec3(const toml::array& arr) -> Vector {
  auto x = arr.at(0).value<Real>();
  auto y = arr.at(1).value<Real>();
  auto z = arr.at(2).value<Real>();

  if (!x.has_value() || !y.has_value() || !z.has_value()) {
    throw XFDTDParseException("tomlArrayToVec3: Invalid array");
  }

  return {x.value(), y.value(), z.value()};
}

}  // namespace xfdtd::parse

#endif  // __XFDTD_PARSE_UTIL_H__
