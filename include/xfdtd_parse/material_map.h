#ifndef __XFDTD_PARSE_MATERIAL_MAP_H__
#define __XFDTD_PARSE_MATERIAL_MAP_H__

#include <xfdtd/common/constant.h>
#include <xfdtd/material/dispersive_material.h>
#include <xfdtd/material/material.h>
#include <xfdtd_parse/exception.h>

#include <toml++/toml.hpp>
#include <unordered_map>

namespace xfdtd {

namespace parse {

class XFDTDParseMaterialMapException : public XFDTDParseException {
 public:
  XFDTDParseMaterialMapException(const std::string& message)
      : XFDTDParseException(message) {}
};

class MaterialMap {
 public:
  constexpr static auto MU_0 = constant::MU_0;
  constexpr static auto EPSILON_0 = constant::EPSILON_0;
  constexpr static auto SIGMA_E_ZERO = constant::SIGMA_E_ZERO_APPROX;
  constexpr static auto SIGMA_M_ZERO = constant::SIGMA_M_ZERO_APPROX;

 public:
  MaterialMap() = default;

  auto read(const toml::table& table) -> void;

  auto map() const
      -> const std::unordered_map<std::string, std::unique_ptr<Material>>& {
    return _materials;
  }

 private:
  auto readMaterial(const toml::table& table) -> void;

  auto makeMaterial(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Material>>;

  auto addMaterial(const std::string& name,
                   std::unique_ptr<Material> material) -> void;

  auto makeCommon(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<Material>>;

  auto makeLinearDispersion(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<LinearDispersiveMaterial>>;

  std::unordered_map<std::string, std::unique_ptr<Material>> _materials;
};

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_MATERIAL_MAP_H__
