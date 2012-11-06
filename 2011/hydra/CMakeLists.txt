cmake_minimum_required(VERSION 2.6.0)

project(hydra)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
find_package(PyInstaller REQUIRED)
include(UsePyInstaller)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(binary_dir ${PROJECT_SOURCE_DIR}/bin)

set(targets server client)
foreach(target ${targets})

    set(script ${CMAKE_PROJECT_NAME}-${target}.py)
    set_source_files_properties(${script} PROPERTIES FILE_DEPLOYMENT ON)
    set_source_files_properties(${script} PROPERTIES DEBUG OFF)
    set_source_files_properties(${script} PROPERTIES WINDOWED OFF)
    set_source_files_properties(${script} PROPERTIES STRIP OFF)

    add_pi_executable(${target} ${script})

    add_custom_command(
        TARGET ${target}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E make_directory
        ARGS ${binary_dir}

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ARGS ${EXECUTABLE_OUTPUT_PATH}/dist ${binary_dir}
    )

endforeach()
