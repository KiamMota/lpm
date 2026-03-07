#include "base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
namespace commands {
void remove(std::vector<std::string> &commands) {
  std::string plugin_to_delete = commands.at(0);

  // remove o arquivo .lua
  for (const std::filesystem::directory_entry &p :
       std::filesystem::directory_iterator(base::nvPath.plugins_path)) {
    if (p.is_regular_file() && p.path().filename() == plugin_to_delete) {
      std::filesystem::remove(p);
    }
  }
  std::cout << "removed." << std::endl;

  // remove o require do init.lua
  std::ifstream file_in(base::nvPath.init_path);
  std::stringstream buffer;
  buffer << file_in.rdbuf();
  file_in.close();
  std::string content = buffer.str();

  std::string require_line = "require('plugins." + plugin_to_delete + "')";
  size_t pos = content.find(require_line);

  if (pos != std::string::npos) {
    // apaga a linha inteira (incluindo o \n)
    size_t line_start = content.rfind('\n', pos);
    line_start = (line_start == std::string::npos) ? 0 : line_start;
    size_t line_end = content.find('\n', pos);
    line_end = (line_end == std::string::npos) ? content.size() : line_end + 1;

    content.erase(line_start, line_end - line_start);

    std::ofstream file_out(base::nvPath.init_path);
    file_out << content;
    file_out.close();
    std::cout << "removed require from init.lua." << std::endl;
  }
}
} // namespace commands
