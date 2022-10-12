cmake_minimum_required(VERSION 3.0.0)
project(ascii-player VERSION 0.1.0)

file(GLOB code_src
    "src/*.c"
)

add_executable(ascii-player ${code_src})

target_include_directories(ascii-player PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

if(NOT WIN32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(ascii-player PRIVATE Threads::Threads)
endif(NOT WIN32)

# add_compile_definitions(DEBUG_TIME) # If you have problems for playback speed enable this and check the time calculated
# add_compile_definitions(DEBUG_TYPE) # Check if you computer is unsing x32 or x64 variables during execution

include(CPack)

install(TARGETS ascii-player DESTINATION bin)
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)