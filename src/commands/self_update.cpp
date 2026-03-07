#include "commands.hpp"
#include <curl/curl.h>
#include <filesystem>
#include <iostream>

namespace commands {

void self_update() {
  std::cout << "checking for updates..." << std::endl;

  std::string binary_path =
      std::filesystem::read_symlink("/proc/self/exe").string();
  std::string download_url =
      "https://github.com/KiamMota/lpm/releases/latest/download/lpm";
  std::string tmp_path = "/tmp/lpm_update";

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cout << "err: failed to init curl" << std::endl;
    return;
  }

  FILE *tmp_file = fopen(tmp_path.c_str(), "wb");
  if (!tmp_file) {
    curl_easy_cleanup(curl);
    std::cout << "err: failed to open temp file" << std::endl;
    return;
  }

  curl_easy_setopt(curl, CURLOPT_URL, download_url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_file);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "lpm/1.0");

  CURLcode res = curl_easy_perform(curl);
  long http_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);
  fclose(tmp_file);

  if (res != CURLE_OK) {
    std::cout << "err: " << curl_easy_strerror(res) << std::endl;
    std::filesystem::remove(tmp_path);
    return;
  }

  if (http_code == 404) {
    std::cout << "no release found." << std::endl;
    std::filesystem::remove(tmp_path);
    return;
  }

  if (http_code != 200) {
    std::cout << "err: unexpected HTTP " << http_code << std::endl;
    std::filesystem::remove(tmp_path);
    return;
  }

  std::filesystem::permissions(tmp_path,
                               std::filesystem::perms::owner_exec |
                                   std::filesystem::perms::group_exec |
                                   std::filesystem::perms::others_exec,
                               std::filesystem::perm_options::add);

  try {
    std::filesystem::rename(tmp_path, binary_path);
    std::cout << "updated! restart lpm." << std::endl;
  } catch (const std::exception &) {
    try {
      std::filesystem::copy_file(
          tmp_path, binary_path,
          std::filesystem::copy_options::overwrite_existing);
      std::filesystem::remove(tmp_path);
      std::cout << "updated! restart lpm." << std::endl;
    } catch (const std::exception &ex) {
      std::cout << "err: need sudo? " << ex.what() << std::endl;
    }
  }
}

} // namespace commands
