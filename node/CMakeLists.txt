cmake_minimum_required (VERSION 3.19)

project (torrent-node)

set (CMAKE_CXX_STANDARD 20)

target_include_directories (${PROJECT_NAME}
  PUBLIC
    lib
)

target_link_libraries (${PROJECT_NAME}
  PUBLIC
    lib
)