
add_executable(FileScanner-CLI main.cpp)
target_link_libraries(FileScanner-CLI PRIVATE FileScanner-DLL)