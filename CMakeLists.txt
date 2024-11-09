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

# Make LevelDB available
FetchContent_MakeAvailable(leveldb)

project(pbl CXX)

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