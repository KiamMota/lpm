#ifndef CMD_INSTALL_HPP
#define CMD_INSTALL_HPP

#include <string>
#include <vector>
namespace commands {
void install(std::vector<std::string> &args);
void remove(std::vector<std::string> &args);
void list(std::vector<std::string> &args);
void install_lazy(std::vector<std::string> &args);
} // namespace commands
#endif // CMD_INSTALL_HPP
