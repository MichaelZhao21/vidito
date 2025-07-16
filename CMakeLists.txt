cmake_minimum_required(VERSION 3.15...4.0)
project(Vidito VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

# Reset library directory

# Download clay.h and clay_renderer_SDL3.c
message(STATUS "Downloading Clay...")
file(DOWNLOAD https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/clay.h lib/clay.h)
file(DOWNLOAD https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/renderers/SDL3/clay_renderer_SDL3.c lib/clay_renderer_SDL3.c)
execute_process(
    COMMAND sed -i "1s/.*/#include \\\"clay.h\\\"/" lib/clay_renderer_SDL3.c
)
message(STATUS "\tDone")

# Download fonts
message(STATUS "Downloading fonts...")
file(DOWNLOAD http://sourceforge.net/projects/dejavu/files/dejavu/2.37/dejavu-fonts-ttf-2.37.zip lib/dejavu.zip)
execute_process(COMMAND unzip -o -qq lib/dejavu.zip -d lib/dejavu)
file(REMOVE_RECURSE lib/fonts)
execute_process(COMMAND mv lib/dejavu/dejavu-fonts-ttf-2.37/ttf lib/fonts)
file(REMOVE lib/dejavu.zip)
file(REMOVE_RECURSE lib/dejavu)
message(STATUS "\tDone")

set(PROJECT_SOURCES
    main.c
)

add_executable(Vidito ${PROJECT_SOURCES})

target_link_libraries(Vidito PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/fonts
        ${CMAKE_CURRENT_BINARY_DIR}/fonts
)
