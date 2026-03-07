#include "base.hpp"
#include <cstdlib>
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

NvimConfigPath nvPath;

} // namespace base
