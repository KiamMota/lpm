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

bool is_nvim_installed() {
#if defined(_WIN32) || defined(_WIN64)
  return std::system("where nvim >nul 2>&1") == 0;
#else
  return std::system("which nvim > /dev/null 2>&1") == 0;
#endif
}
void start_nvPath() {
  base::nvPath.base_path = base::get_nvim_config_path();
  base::nvPath.init_path = base::nvPath.base_path + "/init.lua";
  base::nvPath.lua_path = base::nvPath.base_path + "/lua";
  base::nvPath.config_path = base::nvPath.base_path + "/lua/config";
  base::nvPath.plugins_path = base::nvPath.base_path + "/lua/plugins";
}

NvimConfigPath nvPath;

} // namespace base
