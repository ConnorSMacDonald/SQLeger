find_package(SQLite3 REQUIRED)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_link_libraries(SQLeger INTERFACE SQLite::SQLite3)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/src/$<LOWER_CASE:${PROJECT_NAME}>/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/$<LOWER_CASE:${PROJECT_NAME}>
  DESTINATION include)

if (${SQLEGER_BUILD_TESTS})

  find_package(Catch2 REQUIRED)

  include(CTest)

  add_executable(bind_traits.test bind_traits.test.cpp)
  target_link_libraries(bind_traits.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME bind_traits.test COMMAND bind_traits.test)

  add_executable(binder.test binder.test.cpp)
  target_link_libraries(binder.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME binder.test COMMAND binder.test)

  add_executable(blob_data.test blob_data.test.cpp)
  target_link_libraries(blob_data.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME blob_data.test COMMAND blob_data.test)

  add_executable(column_traits.test column_traits.test.cpp)
  target_link_libraries(column_traits.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME column_traits.test COMMAND column_traits.test)

  add_executable(constants.test constants.test.cpp)
  target_link_libraries(constants.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME constants.test COMMAND constants.test)

  add_executable(db.test db.test.cpp)
  target_link_libraries(db.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME db.test COMMAND db.test)

  add_executable(parameter.test parameter.test.cpp)
  target_link_libraries(parameter.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME parameter.test COMMAND parameter.test)

  add_executable(result_exception.test result_exception.test.cpp)
  target_link_libraries(result_exception.test
    PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME result_exception.test COMMAND result_exception.test)

  add_executable(stmt.test stmt.test.cpp)
  target_link_libraries(stmt.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME stmt.test COMMAND stmt.test)

  add_executable(string.test string.test.cpp)
  target_link_libraries(string.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME string.test COMMAND string.test)

  add_executable(transaction.test transaction.test.cpp)
  target_link_libraries(transaction.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME transaction.test COMMAND transaction.test)

  add_executable(user_blob.test user_blob.test.cpp)
  target_link_libraries(user_blob.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME user_blob.test COMMAND user_blob.test)

  add_executable(user_text.test user_text.test.cpp)
  target_link_libraries(user_text.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME user_text.test COMMAND user_text.test)

  add_executable(utility.test utility.test.cpp)
  target_link_libraries(utility.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME utility.test COMMAND utility.test)

  add_executable(value.test value.test.cpp)
  target_link_libraries(value.test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
  add_test(NAME value.test COMMAND value.test)

endif()
