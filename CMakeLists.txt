cmake_minimum_required(VERSION 3.20)

if(APPLE AND NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED CMAKE_CXX_COMPILER)
  foreach(DANS_VK2_LLVM_PREFIX IN ITEMS /opt/homebrew/opt/llvm /usr/local/opt/llvm)
    if(EXISTS "${DANS_VK2_LLVM_PREFIX}/bin/clang" AND EXISTS "${DANS_VK2_LLVM_PREFIX}/bin/clang++")
      set(CMAKE_C_COMPILER "${DANS_VK2_LLVM_PREFIX}/bin/clang" CACHE FILEPATH "C compiler" FORCE)
      set(CMAKE_CXX_COMPILER "${DANS_VK2_LLVM_PREFIX}/bin/clang++" CACHE FILEPATH "C++ compiler" FORCE)
      message(STATUS "Using LLVM toolchain from ${DANS_VK2_LLVM_PREFIX}")
      break()
    endif()
  endforeach()
endif()

project(dans_vk2 VERSION 0.1.0 LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(DANS_VK2_IS_TOP_LEVEL ON)
else()
  set(DANS_VK2_IS_TOP_LEVEL OFF)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MSVC's CRT headers mark fopen/strcpy/etc. deprecated in favour of their
# non-standard _s variants; under -Werror that breaks the build on Windows.
if(WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Force glm's length_type (vector/matrix operator[] index and .length()) to
# std::size_t so glm indexing matches usize everywhere. Must be global and
# identical for every TU or glm types mismatch across translation units.
add_compile_definitions(GLM_FORCE_SIZE_T_LENGTH)

option(DANS_VK2_ENABLE_PCH "Build targets with precompiled headers" ON)

add_library(dans_vk2_warnings INTERFACE)
target_compile_options(dans_vk2_warnings INTERFACE
  $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wconversion
    -Wfloat-conversion
    -Wsign-conversion
    -Wcast-qual
    -Wcast-align
    -Wformat=2
    -Wundef
    -Wnull-dereference
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wold-style-cast
    -Wzero-as-null-pointer-constant
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wreturn-type>
  $<$<CXX_COMPILER_ID:Clang,AppleClang>:
    -Wconditional-uninitialized
    -Wextra-semi
    -Wimplicit-int-conversion
    -Wimplicit-int-float-conversion
    -Wnewline-eof
    -Wshorten-64-to-32
    -Wswitch-enum
    -Wunreachable-code>
  $<$<CXX_COMPILER_ID:GNU>:
    -Wduplicated-branches
    -Wduplicated-cond
    -Wlogical-op
    -Wsign-compare
    -Wuseless-cast>
)

# --- dans foundation (header-only) -----------------------------------------
# Public foundation headers (<dans/types.hpp>, <dans/pch.hpp>,
# <dans/development_markers.hpp>) are a vendored snapshot of dans-platform under
# vendor/dans. The generic utility headers (<dans/chrono.hpp>, <dans/format.hpp>,
# ...) live under src/ and are added to the app's private include path below.
add_library(dans_core INTERFACE)
add_library(dans::core ALIAS dans_core)
target_include_directories(dans_core INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor>
)

# --- vendored dependencies --------------------------------------------------
if(NOT TARGET glm::glm)
  add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)
endif()

# GLFW: windowing/input. Static library only; examples/tests/docs disabled.
# SYSTEM keeps glfw3.h out of reach of dans_vk2_warnings' -Werror flags.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
if(NOT TARGET glfw)
  add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL SYSTEM)
endif()

# glfw3.h includes <vulkan/vulkan.h> and declares the Vulkan interop functions
# (glfwCreateWindowSurface, ...) only when GLFW_INCLUDE_VULKAN is set, and never
# pulls OpenGL headers. Apply it as an INTERFACE definition so it reaches glfw's
# consumers (the app, and later the framework) but not GLFW's own translation
# units, which reject any GLFW_INCLUDE_* macro.
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_VULKAN)

find_package(Vulkan REQUIRED)

find_program(DANS_VK2_GLSLC_EXECUTABLE glslc HINTS "$ENV{VULKAN_SDK}/bin")
if(NOT DANS_VK2_GLSLC_EXECUTABLE)
  message(FATAL_ERROR "Vulkan was found, but glslc was not. Set VULKAN_SDK or install Vulkan shader tools.")
endif()

# Vulkan Memory Allocator: header-only. Add a single .cpp defining
# VMA_IMPLEMENTATION to your framework when you wire allocation up.
add_library(dans_vk2_vma INTERFACE)
target_include_directories(dans_vk2_vma SYSTEM INTERFACE vendor/vma/include)
target_link_libraries(dans_vk2_vma INTERFACE Vulkan::Vulkan)

# Header-only helpers, exposed as include dirs for when you reach them.
add_library(dans_vk2_stb INTERFACE)
target_include_directories(dans_vk2_stb SYSTEM INTERFACE vendor/stb)
add_library(dans_vk2_json INTERFACE)
target_include_directories(dans_vk2_json SYSTEM INTERFACE vendor/nlohmann)

add_library(dans_vk2_gsl INTERFACE)
target_include_directories(dans_vk2_gsl SYSTEM INTERFACE vendor/gsl/include)

# Dear ImGui (core only). Backends are intentionally not vendored: you write
# imgui_impl_vulkan yourself, and drop a platform backend (imgui_impl_glfw) into
# vendor/imgui/backends/ when you build the UI. Add those .cpp here and link
# dans_vk2_imgui then. Built on demand (EXCLUDE_FROM_ALL) until referenced.
add_library(dans_vk2_imgui STATIC EXCLUDE_FROM_ALL
  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_tables.cpp
  vendor/imgui/imgui_widgets.cpp
)
target_include_directories(dans_vk2_imgui SYSTEM PUBLIC vendor/imgui)
target_link_libraries(dans_vk2_imgui PUBLIC glfw Vulkan::Vulkan)

# --- shaders (glslc -> SPIR-V) ---------------------------------------------
# Drop .vert/.frag/.comp sources under src/vk/shaders, list them here, and
# each compiles to ${CMAKE_BINARY_DIR}/shaders/<name>.spv at build time. Empty
# until you add shaders.
set(DANS_VK2_SHADER_SOURCES)
set(DANS_VK2_SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(DANS_VK2_SHADER_OUTPUTS)
foreach(DANS_VK2_SHADER ${DANS_VK2_SHADER_SOURCES})
  get_filename_component(DANS_VK2_SHADER_NAME "${DANS_VK2_SHADER}" NAME)
  set(DANS_VK2_SHADER_OUTPUT "${DANS_VK2_SHADER_OUTPUT_DIR}/${DANS_VK2_SHADER_NAME}.spv")
  add_custom_command(
    OUTPUT "${DANS_VK2_SHADER_OUTPUT}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DANS_VK2_SHADER_OUTPUT_DIR}"
    COMMAND "${DANS_VK2_GLSLC_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/${DANS_VK2_SHADER}" -o "${DANS_VK2_SHADER_OUTPUT}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${DANS_VK2_SHADER}"
    VERBATIM
  )
  list(APPEND DANS_VK2_SHADER_OUTPUTS "${DANS_VK2_SHADER_OUTPUT}")
endforeach()
if(DANS_VK2_SHADER_OUTPUTS)
  add_custom_target(dans_vk2_shaders DEPENDS ${DANS_VK2_SHADER_OUTPUTS})
endif()

# --- precompiled header -----------------------------------------------------
function(dans_vk2_enable_pch target_name)
  if(DANS_VK2_ENABLE_PCH AND TARGET ${target_name})
    target_precompile_headers(${target_name} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src/vk/pch.hpp
    )
  endif()
endfunction()

# --- application ------------------------------------------------------------
add_executable(dans_vk2
  app/main.cpp
  src/platform/platform.cpp
  src/vk/vk_config.cpp
  src/vk/vk_context.cpp
)
target_include_directories(dans_vk2 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(dans_vk2 PRIVATE
  dans::core
  glm::glm
  glfw
  Vulkan::Vulkan
  dans_vk2_vma
  dans_vk2_gsl
  dans_vk2_warnings
)
dans_vk2_enable_pch(dans_vk2)

# Optimized builds turn off Vulkan validation: the app gates its DebugMessengerCfg
# on this define (see app/main.cpp), so the validation layer is never requested.
# run_opt.sh passes -DDANS_VK2_NO_VALIDATION=ON; dev/dbg leave it OFF.
option(DANS_VK2_NO_VALIDATION "Disable Vulkan validation layers" OFF)
if(DANS_VK2_NO_VALIDATION)
  target_compile_definitions(dans_vk2 PRIVATE DANS_VK2_NO_VALIDATION)
endif()
