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

class MaterialEntry {
 public:
  MaterialEntry(const std::string& name, Real epsilon_r, Real mu_r,
                Real sigma_e, Real sigma_m)
      : _name{name},
        _epsilon_r{epsilon_r},
        _mu_r{mu_r},
        _sigma_e{sigma_e},
        _sigma_m{sigma_m} {}

  MaterialEntry(const MaterialEntry& other) = default;

  MaterialEntry(MaterialEntry&& other) noexcept = default;

  MaterialEntry& operator=(const MaterialEntry& other) = default;

  MaterialEntry& operator=(MaterialEntry&& other) noexcept = default;

  virtual ~MaterialEntry() = default;

  auto name() const -> const std::string& { return _name; }

  auto epsilonR() const -> Real { return _epsilon_r; }

  auto muR() const -> Real { return _mu_r; }

  auto sigmaE() const -> Real { return _sigma_e; }

  auto sigmaM() const -> Real { return _sigma_m; }

  virtual auto toString() const -> std::string {
    std::stringstream ss;
    ss << "Material: {" << _name << ", ";
    ss << "Relative Permittivity: " << _epsilon_r << ", ";
    ss << "Relative Permeability: " << _mu_r << ", ";
    ss << "Electric Conductivity: " << _sigma_e << ", ";
    ss << "Magnetic Conductivity: " << _sigma_m << "}";
    return ss.str();
  }

  virtual auto make() const -> std::unique_ptr<Material> {
    return std::make_unique<Material>(
        _name, ElectroMagneticProperty{_epsilon_r, _mu_r, _sigma_e, _sigma_m});
  }

 private:
  std::string _name{};
  Real _epsilon_r{};
  Real _mu_r{};
  Real _sigma_e{};
  Real _sigma_m{};
};

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

  using Record =
      std::unordered_map<std::string, std::unique_ptr<MaterialEntry>>;

 public:
  MaterialMap() = default;

  auto read(const toml::table& table) -> void;

  auto map() const -> const Record& { return _materials; }

 private:
  auto readMaterial(const toml::table& table) -> void;

  auto makeMaterialEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<MaterialEntry>>;

  auto addMaterialEntry(const std::string& name,
                        std::unique_ptr<MaterialEntry> material) -> void;

  auto makeCommonEntry(const toml::table& table) const
      -> std::tuple<std::string, std::unique_ptr<MaterialEntry>>;

  Record _materials;
};

}  // namespace parse

}  // namespace xfdtd

#endif  // __XFDTD_PARSE_MATERIAL_MAP_H__
