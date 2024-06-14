#include <xfdtd/material/material.h>
#include <xfdtd_parse/material_map.h>

namespace xfdtd {

namespace parse {

auto MaterialMap::read(const toml::table& table) -> void {
  readMaterial(table);
}

auto MaterialMap::readMaterial(const toml::table& table) -> void {
  auto material_arr = table.get_as<toml::array>("material");
  if (!material_arr) {
    throw XFDTDParseMaterialMapException(
        "readMaterial: Missing field material");
  }

  for (const auto& material : *material_arr) {
    auto material_table = material.as_table();
    if (!material_table) {
      throw XFDTDParseMaterialMapException(
          "readMaterial: Material is not a table");
    }

    try {
      auto [name, material_ptr] = makeMaterialEntry(*material_table);
      addMaterialEntry(name, std::move(material_ptr));
    } catch (const XFDTDParseMaterialMapException& e) {
      std::stringstream ss;
      ss << "Warning: occurred exception in readMaterial: " << e.what()
         << ". Skip this material.\n";
      std::cerr << ss.str();
    }
  }
}

auto MaterialMap::makeMaterialEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<MaterialEntry>> {
  auto is_dispersive = table["dispersive"].value<bool>();
  if (!is_dispersive.has_value() || !is_dispersive.value()) {
    return makeCommonEntry(table);
  }

  throw XFDTDParseMaterialMapException(
      "MaterialMap::makeMaterialEntry: Not implemented");
}

auto MaterialMap::addMaterialEntry(
    const std::string& name, std::unique_ptr<MaterialEntry> material) -> void {
  _materials[name] = std::move(material);
}

auto MaterialMap::makeCommonEntry(const toml::table& table) const
    -> std::tuple<std::string, std::unique_ptr<MaterialEntry>> {
  auto name = table["name"].value<std::string>();
  if (!name) {
    std::stringstream ss;
    ss << "readMaterial: Missing field name";
    throw XFDTDParseMaterialMapException(ss.str());
  }

  auto epsilon_r = table["relative_permittivity"].value<double>();
  if (!epsilon_r) {
    std::stringstream ss;
    ss << "readMaterial: Missing field relative_permittivity in Material: "
       << name.value();
    ss << ". Using default value of 1.0.";
    epsilon_r = 1.0;
  }

  auto mu_r = table["relative_permeability"].value<double>();
  if (!mu_r) {
    std::stringstream ss;
    ss << "readMaterial: Missing field relative_permeability in Material: "
       << name.value();
    ss << ". Using default value of 1.0.";
    mu_r = 1.0;
  }

  auto sigma_e = table["electric_conductivity"].value<double>();
  if (!sigma_e) {
    std::stringstream ss;
    ss << "readMaterial: Missing field electric_conductivity in Material: "
       << name.value();
    ss << ". Using default value of 0.0.";
    sigma_e = 0.0;
  }

  auto sigma_m = table["magnetic_conductivity"].value<double>();
  if (!sigma_m) {
    std::stringstream ss;
    ss << "readMaterial: Missing field magnetic_conductivity in Material: "
       << name.value();
    ss << ". Using default value of 0.0.";
    sigma_m = 0.0;
  }

  return std::make_tuple(name.value(),
                         std::make_unique<MaterialEntry>(
                             name.value(), epsilon_r.value(), mu_r.value(),
                             sigma_e.value(), sigma_m.value()));
}

}  // namespace parse

}  // namespace xfdtd
