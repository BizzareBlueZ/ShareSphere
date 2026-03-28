cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/permissive->")
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(sharesphere_qt
    main_qt.cpp
    User.cpp
    Student.cpp
    Item.cpp
    Request.cpp
    FileManager.cpp
    Validator.cpp
)

target_link_libraries(sharesphere_qt Qt6::Widgets)
target_include_directories(sharesphere_qt PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)