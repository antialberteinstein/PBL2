cmake_minimum_required(VERSION 3.15)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

project(pbl CXX)

# Use SQLite3
file(GLOB_RECURSE SQLITE3_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Librares/sqlite3/*.c)


include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(pbl ${SOURCES} ${SQLITE3_SOURCES})

target_link_libraries(pbl
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

FILE(COPY "${CMAKE_SOURCE_DIR}/res/" DESTINATION "${CMAKE_BINARY_DIR}/res/")