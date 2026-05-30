cmake_minimum_required(VERSION 3.14)
project(nnet VERSION 0.1.0 LANGUAGES C)

# ── Standards ────────────────────────────────────────────────────────────────
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ── Options ──────────────────────────────────────────────────────────────────
option(NNET_BUILD_EXAMPLES "Build the bundled examples" OFF)
option(NNET_INSTALL        "Generate install targets"   ON)

# ── Library ──────────────────────────────────────────────────────────────────
file(GLOB_RECURSE NNET_SOURCES "src/*.c")

add_library(nnet STATIC ${NNET_SOURCES})
add_library(nnet::nnet ALIAS nnet)

# Flags are PRIVATE so they don't leak into consumer builds
target_compile_options(nnet PRIVATE -Wall -Wextra -O2)

target_include_directories(nnet PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link math library
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(nnet PUBLIC ${MATH_LIBRARY})
endif()

# ── Examples ─────────────────────────────────────────────────────────────────
# Only built when explicitly requested (not when used as a dependency)
if(NNET_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ── Install ──────────────────────────────────────────────────────────────────
if(NNET_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install the library archive
    install(TARGETS nnet
        EXPORT  nnetTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    # Install the headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
    )

    # Export the targets file
    install(EXPORT nnetTargets
        FILE      nnetTargets.cmake
        NAMESPACE nnet::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nnet
    )

    # Generate nnetConfig.cmake
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nnetConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/nnetConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nnet
    )

    # Generate nnetConfigVersion.cmake
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/nnetConfigVersion.cmake"
        VERSION       ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/nnetConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/nnetConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nnet
    )

    # ── Uninstall Target ──────────────────────────────────────────────────────
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -E echo "Uninstalling files from install_manifest.txt..."
        COMMAND xargs rm -f < ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt
        COMMAND ${CMAKE_COMMAND} -E rm -rf
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/nnet
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/nnet
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Uninstalling nnet"
    )
endif()