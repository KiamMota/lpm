#ifndef CMD_INSTALL_HPP
#define CMD_INSTALL_HPP

#include <string>
#include <vector>
namespace commands {
void install(std::vector<std::string> &args);
void remove(std::vector<std::string> &args);
void list();
void install_lazy();
void include(std::vector<std::string> &args);
} // namespace commands

namespace commands {
namespace utils {
const std::vector<std::string> fuzzy_find_plugin(const char *plugin_name);
void insert_require_in_init_lua(const std::string &content);
} // namespace utils
} // namespace commands

#endif // CMD_INSTALL_HPP
