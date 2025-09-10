find_package(GTest REQUIRED)

add_executable(FileScanner-Tests test_base.cpp test_logger.cpp test_scanner.cpp)
target_link_libraries(FileScanner-Tests PRIVATE FileScanner-DLL GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(FileScanner-Tests)

install(TARGETS FileScanner-Tests RUNTIME DESTINATION bin)