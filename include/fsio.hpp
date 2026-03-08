#ifndef FSIO_HPP
#define FSIO_HPP

#include <string>
#include <vector>

namespace fsio {

std::vector<std::string> read_lines(const std::string &path);
const std::string read_file(const std::string &path);
void write_lines(const std::string &path,
                 const std::vector<std::string> &lines);
void write_file(const std::string &path, const std::string &content);
std::string get_indent(const std::vector<std::string> &lines, int index);
int find_last_closing_brace(const std::vector<std::string> &lines);

} // namespace fsio

#endif // FSIO_HPP
