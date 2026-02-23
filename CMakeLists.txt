cmake_minimum_required(VERSION 3.12)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

# Fetch LevelDB
FetchContent_Declare(
    leveldb
    GIT_REPOSITORY https://github.com/google/leveldb.git
    GIT_TAG        main # You can specify a tag or commit if needed
)

# Force the C++ standard to be passed to LevelDB
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to build all targets." FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require specified C++ standard" FORCE)
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Disable compiler specific extensions" FORCE)

set(LEVELDB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

# Make LevelDB available
FetchContent_MakeAvailable(leveldb)

project(pbl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(pbl ${SOURCES})

# include_directories(${CMAKE_SOURCE_DIR}/include)
target_include_directories(pbl
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(pbl
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  leveldb
)

# Copy res folder to build directory
FILE(COPY "${CMAKE_SOURCE_DIR}/res/" DESTINATION "${CMAKE_BINARY_DIR}/res/")