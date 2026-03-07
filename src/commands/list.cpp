#include "base.hpp"
#include "commands.hpp"
#include <filesystem>
#include <iostream>

namespace commands {

void list(std::vector<std::string> &commands) {

  std::cout << "installed plugins in " << base::nvPath.plugins_path << ": "
            << std::endl;

  for (const std::filesystem::directory_entry &p :
       std::filesystem::directory_iterator(base::nvPath.plugins_path)) {
    if (p.is_regular_file())
      std::cout << p.path().filename() << std::endl;
  }
}
} // namespace commands
