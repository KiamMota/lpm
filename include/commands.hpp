#ifndef CMD_INSTALL_HPP
#define CMD_INSTALL_HPP

#include <string>
#include <vector>
namespace commands {
void install_lazy();
void list();
void install(std::vector<std::string> &args);
void remove(std::vector<std::string> &args);
void cat(std::vector<std::string> &args);
void edit(std::vector<std::string> &args);
void able(std::vector<std::string> &args);
void deactivate(std::vector<std::string> &args);
void self_update();

} // namespace commands

namespace commands {
namespace utils {
const std::vector<std::string> fuzzy_find_plugin(const char *plugin_name);
const std::string
when_find_multiple_plugins(std::vector<std::string> &plugns_found);
void insert_require_in_init_lua(const std::string &content);
void comment_require_line_in_init_lua(std::string &content);
void decomment_require_line_in_init_lua(std::string &content);
} // namespace utils
} // namespace commands

#endif // CMD_INSTALL_HPP
