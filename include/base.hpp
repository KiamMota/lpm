#ifndef BASE_HPP
#define BASE_HPP

#include <string>

#define VERSION_STR "1.0.0"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0
namespace base {
const std::string get_nvim_config_path();
struct NvimConfigPath {
  std::string base_path;
  std::string init_path;
  std::string lua_path;
  std::string plugins_path;
  std::string config_path;
  bool is_lazyvim;
};

extern NvimConfigPath nvPath;

} // namespace base

#endif // BASE_HPP
