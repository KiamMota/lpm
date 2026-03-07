#ifndef CLI_UTILS_HPP
#define CLI_UTILS_HPP

#include <string>
#include <vector>
namespace cli {

extern std::vector<std::string> main_args;

void help();
void version();
void require_arg(const char *cmd, const char *required_arg);
bool msg_question(const std::string &msg);

} // namespace cli

#endif // CLI_UTILS_HPP
