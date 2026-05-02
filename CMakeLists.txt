cmake_minimum_required(VERSION 3.20)
project(implicit_treap CXX)

# ---------- Estándar ----------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------- compile_commands.json para clangd ----------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Build type por defecto: Release ----------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---------- Flags ----------
set(WARN_FLAGS -Wall -Wextra -Wshadow -Wno-unused-result)

# Sanitizers solo en Debug
set(SANITIZER_FLAGS -fsanitize=address,undefined)

# ---------- Ejecutable ----------
add_executable(main
    src/main.cpp
    src/treap.cpp
)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE
    ${WARN_FLAGS}
    $<$<CONFIG:Release>:-O2 -DNDEBUG>
    $<$<CONFIG:Debug>:-O0 -g ${SANITIZER_FLAGS} -D_GLIBCXX_DEBUG>
)

target_link_options(main PRIVATE
    $<$<CONFIG:Debug>:${SANITIZER_FLAGS}>
)

# ---------- Custom targets para comodidad ----------

# Symlink de compile_commands.json a la raíz del proyecto
# (clangd lo busca primero ahí)
add_custom_target(symlink_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Linking compile_commands.json to project root"
)

# make run -> ejecuta con sample.in
add_custom_target(run
    COMMAND $<TARGET_FILE:main> < ${CMAKE_SOURCE_DIR}/tests/sample.in
    DEPENDS main
    USES_TERMINAL
)

# make test -> compara contra sample.out
add_custom_target(test_sample
    COMMAND $<TARGET_FILE:main> < ${CMAKE_SOURCE_DIR}/tests/sample.in
            > ${CMAKE_BINARY_DIR}/sample.got
    COMMAND ${CMAKE_COMMAND} -E compare_files
            ${CMAKE_SOURCE_DIR}/tests/sample.out
            ${CMAKE_BINARY_DIR}/sample.got
    DEPENDS main
    COMMENT "Running sample test"
    USES_TERMINAL
)

# ---------- Targets de testing ----------

add_executable(brute   tests/brute.cpp)
add_executable(gen     tests/gen.cpp)
add_executable(gen_big tests/gen_big.cpp)

target_compile_options(brute   PRIVATE ${WARN_FLAGS} -O2)
target_compile_options(gen     PRIVATE ${WARN_FLAGS} -O2)
target_compile_options(gen_big PRIVATE ${WARN_FLAGS} -O2)

# Stress test: requiere main, brute y gen ya compilados
add_custom_target(stress
    COMMAND ${CMAKE_SOURCE_DIR}/tests/stress.sh 200
    DEPENDS main brute gen
    USES_TERMINAL
)

# Benchmark con caso grande
add_custom_target(bench
    COMMAND ${CMAKE_SOURCE_DIR}/tests/bench.sh
    DEPENDS main gen_big
    USES_TERMINAL
)
