cmake_minimum_required(VERSION 3.16)
project(lazynvimcli CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS        OFF)
set(CURL_STATICLIB           ON)
set(BUILD_CURL_EXE           OFF)
set(HTTP_ONLY                ON)
set(CURL_USE_OPENSSL         ON)
set(CURL_DISABLE_TESTS       ON)
set(BUILD_TESTING            OFF)
set(CURL_DISABLE_DOCS        ON)
set(CURL_DISABLE_EXAMPLES    ON)
set(CURL_ENABLE_EXPORT_TARGET OFF)
set(CURL_DISABLE_INSTALL     ON)
add_subdirectory(third_party/curl)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(lazynvimcli ${SRC_FILES})

target_include_directories(lazynvimcli PRIVATE
    third_party/curl/include
    include
)

target_link_libraries(lazynvimcli PRIVATE libcurl_static)

if(WIN32)
    target_link_libraries(lazynvimcli PRIVATE ws2_32 wldap32 crypt32 normaliz)
endif()

if(MSVC)
    target_compile_options(lazynvimcli PRIVATE /W4 /WX)
else()
    target_compile_options(lazynvimcli PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set_target_properties(lazynvimcli PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-std=c++17"
    )
endif()

install(TARGETS lazynvimcli RUNTIME DESTINATION bin)
