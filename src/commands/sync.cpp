#include "commands.hpp"
#include <iostream>

namespace commands {

void sync() {
  std::cout << "syncing..." << std::endl;
#if defined(_WIN32) || defined(_WIN64)
  int result =
      std::system("nvim --headless -c \"Lazy sync\" -c \"qa\" >nul 2>&1");
#else
  int result =
      std::system("nvim --headless -c 'Lazy sync' -c 'qa' >/dev/null 2>&1");
#endif
  if (result != 0)
    std::cout << "sync failed." << std::endl;
  else
    std::cout << "done." << std::endl;
}
} // namespace commands
