#include "commands.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace commands {

static void log(const std::string &msg) {
  std::cout << "[lazy-install] " << msg << std::endl;
}

static void log_error(const std::string &msg) {
  std::cerr << "[lazy-install] ERROR: " << msg << std::endl;
}

static bool run_command(const std::string &cmd) {
  log("Running: " + cmd);
  int result = std::system(cmd.c_str());
  if (result != 0) {
    log_error("Command failed with exit code: " + std::to_string(result));
    return false;
  }
  return true;
}

static fs::path get_lazy_path() {
#ifdef _WIN32
  const char *appdata = std::getenv("LOCALAPPDATA");
  if (!appdata)
    throw std::runtime_error("LOCALAPPDATA environment variable not found.");
  return fs::path(appdata) / "nvim-data" / "lazy" / "lazy.nvim";
#else
  const char *home = std::getenv("HOME");
  if (!home)
    throw std::runtime_error("HOME environment variable not found.");
  return fs::path(home) / ".local" / "share" / "nvim" / "lazy" / "lazy.nvim";
#endif
}

static bool git_available() {
#ifdef _WIN32
  return std::system("git --version >nul 2>&1") == 0;
#else
  return std::system("git --version >/dev/null 2>&1") == 0;
#endif
}

void install_lazy() {
  log("Starting lazy.nvim installation...");

  // Check if git is available
  if (!git_available()) {
    log_error("git is not installed or not in PATH. Please install git first.");
    return;
  }
  log("git found.");

  fs::path lazy_path;
  try {
    lazy_path = get_lazy_path();
  } catch (const std::exception &e) {
    log_error(e.what());
    return;
  }

  log("Target install path: " + lazy_path.string());

  // Check if already installed
  if (fs::exists(lazy_path)) {
    log("lazy.nvim is already installed at: " + lazy_path.string());
    log("Pulling latest changes...");

#ifdef _WIN32
    std::string pull_cmd = "git -C \"" + lazy_path.string() + "\" pull";
#else
    std::string pull_cmd = "git -C '" + lazy_path.string() + "' pull";
#endif
    if (run_command(pull_cmd)) {
      log("lazy.nvim updated successfully.");
    } else {
      log_error("Failed to update lazy.nvim.");
    }
    return;
  }

  // Create parent directories if they don't exist
  fs::path parent = lazy_path.parent_path();
  try {
    fs::create_directories(parent);
    log("Created directories: " + parent.string());
  } catch (const std::exception &e) {
    log_error("Failed to create directories: " + std::string(e.what()));
    return;
  }

  // Clone lazy.nvim
  const std::string repo_url = "https://github.com/folke/lazy.nvim.git";
  const std::string branch = "stable";

#ifdef _WIN32
  std::string clone_cmd = "git clone --filter=blob:none --branch " + branch +
                          " \"" + repo_url + "\" \"" + lazy_path.string() +
                          "\"";
#else
  std::string clone_cmd = "git clone --filter=blob:none --branch " + branch +
                          " '" + repo_url + "' '" + lazy_path.string() + "'";
#endif

  log("Cloning lazy.nvim from " + repo_url + " (branch: " + branch + ")...");
  if (run_command(clone_cmd)) {
    log("lazy.nvim installed successfully at: " + lazy_path.string());
    log("Add the following to your init.lua to bootstrap lazy.nvim:");
    log("  vim.opt.rtp:prepend(vim.fn.stdpath('data') .. '/lazy/lazy.nvim')");
  } else {
    log_error("Failed to clone lazy.nvim.");

    // Cleanup partial clone if it exists
    if (fs::exists(lazy_path)) {
      log("Cleaning up partial installation...");
      fs::remove_all(lazy_path);
    }
  }
}

} // namespace commands
