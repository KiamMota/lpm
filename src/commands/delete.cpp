#include "base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace commands {
void remove(std::vector<std::string> &commands) {
  std::string plugin_to_delete = commands.at(0);
  for (const std::filesystem::directory_entry &p :
       std::filesystem::directory_iterator(base::nvPath.plugins_path)) {
    if (p.is_regular_file()) {
      if (p.path().filename() == plugin_to_delete) {
        std::filesystem::remove(p);
      }
    }
  }

  std::cout << "removed." << std::endl;
  std::ifstream file(base::nvPath.init_path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  bool found = content.find("require('plugins." + plugin_to_delete + "')") !=
               std::string::npos;
}
} // namespace commands
