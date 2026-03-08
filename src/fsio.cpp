#include "fsio.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fsio {

std::vector<std::string> read_lines(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("failed to open: " + path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line))
    lines.push_back(line);
  return lines;
}

void write_lines(const std::string &path,
                 const std::vector<std::string> &lines) {
  std::ofstream file(path);
  if (!file.is_open())
    throw std::runtime_error("failed to open for writing: " + path);
  for (const std::string &l : lines) {
    file << l << "\n";
    if (file.fail())
      throw std::runtime_error("failed to write line to: " + path);
  }
}

void write_file(const std::string &path, const std::string &content) {
  std::ofstream file(path);
  if (!file.is_open())
    throw std::runtime_error("failed to open for writing: " + path);
  file << content;
  if (file.fail())
    throw std::runtime_error("failed to write line to: " + path);
}

const std::string read_file(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("failed to open: " + path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string get_indent(const std::vector<std::string> &lines, int index) {
  if (index <= 0)
    return "  ";
  std::string indent;
  for (char c : lines[index - 1]) {
    if (c == ' ' || c == '\t')
      indent += c;
    else
      break;
  }
  return indent.empty() ? "  " : indent;
}

int find_last_closing_brace(const std::vector<std::string> &lines) {
  for (int i = static_cast<int>(lines.size()) - 1; i >= 0; i--)
    if (lines[i].find('}') != std::string::npos)
      return i;
  return -1;
}

} // namespace fsio
