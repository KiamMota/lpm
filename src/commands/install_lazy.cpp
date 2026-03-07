#include "base.hpp"
#include "commands.hpp"
#include "lazy_config.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace commands {
void install_lazy() {
  std::cout << "Lazy installation for a lazy plugin manager...\n";

  std::string lazy_file_path = base::nvPath.config_path + "/lazy.lua";

  try {
    if (!std::filesystem::exists(base::nvPath.lua_path))
      std::filesystem::create_directory(base::nvPath.lua_path);

    if (!std::filesystem::exists(base::nvPath.config_path))
      std::filesystem::create_directory(base::nvPath.config_path);

    if (!std::filesystem::exists(lazy_file_path)) {
      std::ofstream file(lazy_file_path);
      if (!file.is_open())
        throw std::runtime_error("Failed to create lazy.lua");
      file << LAZY_LUA_CONTENT;
      file.close();
    }

    if (!std::filesystem::exists(base::nvPath.plugins_path))
      std::filesystem::create_directory(base::nvPath.plugins_path);

    std::ofstream init(base::nvPath.init_path, std::ios::app);
    if (!init.is_open())
      throw std::runtime_error("Failed to open init.lua");
    init << "\nrequire(\"config.lazy\")\n";
    init.close();

    std::cout << "Lazy installed successfully!\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << " — rolling back...\n";

    // rollback
    if (std::filesystem::exists(lazy_file_path))
      std::filesystem::remove(lazy_file_path);

    if (std::filesystem::exists(base::nvPath.plugins_path))
      std::filesystem::remove(base::nvPath.plugins_path);

    if (std::filesystem::exists(base::nvPath.config_path))
      std::filesystem::remove(base::nvPath.config_path);

    if (std::filesystem::exists(base::nvPath.lua_path))
      std::filesystem::remove(base::nvPath.lua_path);

    std::cerr << "Rollback complete.\n";
  }
}
} // namespace commands
