cmake_minimum_required(VERSION 3.15...4.0)
project(Vidito VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

# Download clay.h and clay_renderer_SDL3.c
file(DOWNLOAD https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/clay.h lib/clay.h)
file(DOWNLOAD https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/renderers/SDL3/clay_renderer_SDL3.c lib/clay_renderer_SDL3.c)
execute_process(
    COMMAND sed -i "1s/.*/#include \\\"clay.h\\\"/" lib/clay_renderer_SDL3.c
)

set(PROJECT_SOURCES
    main.c
)

add_executable(Vidito ${PROJECT_SOURCES})

target_link_libraries(Vidito PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
)
