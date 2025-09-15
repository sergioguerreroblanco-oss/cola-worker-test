cmake_minimum_required(VERSION 3.16)
project(cola_worker_test 
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Librería principal con la lógica de Cola y Worker ===
add_library(core STATIC
    src/cola.cpp
    src/worker.cpp
)
target_include_directories(core PUBLIC include)

# Configuración de warnings por compilador
if(MSVC)
    target_compile_options(core PRIVATE /W4 /permissive-)
else()
    target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# === Ejecutable principal ===
add_executable(cola_worker src/main.cpp)
target_link_libraries(cola_worker PRIVATE core)

# === Tests ===
include(CTest)
if(BUILD_TESTING)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(tests tests/test_main.cpp)
    target_link_libraries(tests PRIVATE core gtest_main)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
