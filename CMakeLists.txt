cmake_minimum_required(VERSION 3.12)

project(SQLeger
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "A C++ wrapper around SQLite"
    HOMEPAGE_URL "https://github.com/Adjacence/SQLeger")

if (NOT SQLEGER_BUILD_TESTS)
  set(SQLEGER_BUILD_TESTS_DEFAULT "OFF")

  if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(SQLEGER_BUILD_TESTS_DEFAULT "ON")
  endif()

  set(SQLEGER_BUILD_TESTS
    ${SQLEGER_BUILD_TESTS_DEFAULT}
    CACHE
    BOOL
    "Toggle building of Ieme tests")
endif()

enable_testing()

add_subdirectory(src)
