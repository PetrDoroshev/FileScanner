
set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_library(FileScanner-DLL SHARED  FileScanner.cpp
                                    FileScanner.h
                                    Logger.cpp
                                    Logger.h
                                    HashDataBase.h 
                                    HashDataBase.cpp
                                    ThreadPool.h
                                    MD5.h)


target_include_directories(FileScanner-DLL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


