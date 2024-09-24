cmake_minimum_required(VERSION 3.15)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

project(pbl CXX)

# Install sqlite3
if (WIN32)
  set(SQLite3_DIR ${CMAKE_SOURCE_DIR}/Libraries/sqlite3)
  set(SQLite3_INCLUDE_DIR ${SQLite3_DIR})
  set(SQLite3_LIBRARY ${SQLite3_DIR}/sqlite3.lib)
endif()

set(BUILD_SHARED_LIBS OFF)

find_package(SQLite3 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(pbl ${SOURCES})

# include_directories(${CMAKE_SOURCE_DIR}/include)
target_include_directories(pbl
  PRIVATE ${CMAKE_SOURCE_DIR}/include
  PRIVATE ${SQLite3_INCLUDE_DIR}
)


target_link_libraries(pbl
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE ${SQLite3_LIBRARY}
)

# Copy res folder to build directory
FILE(COPY "${CMAKE_SOURCE_DIR}/res/" DESTINATION "${CMAKE_BINARY_DIR}/res/")