#include "base.hpp"
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>

namespace base {

const std::string get_nvim_config_path() {
#ifdef _WIN32
  const char *appdata = std::getenv("LOCALAPPDATA");
  if (!appdata)
    throw std::runtime_error("LOCALAPPDATA not found");
  return std::string(appdata) + "\\nvim";
#else
  const char *home = std::getenv("HOME");
  if (!home)
    throw std::runtime_error("HOME not found");
  return std::string(home) + "/.config/nvim";
#endif
}

bool is_lazy_installed() {
  return std::filesystem::exists(base::nvPath.lua_path) &&
         std::filesystem::exists(base::nvPath.config_path) &&
         std::filesystem::exists(base::nvPath.plugins_path) &&
         std::filesystem::exists(base::nvPath.config_path + "/lazy.lua") &&
         std::filesystem::exists(base::nvPath.init_path);
}

NvimConfigPath nvPath;

} // namespace base
