cmake_minimum_required(VERSION 3.14)

project(2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Sql )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

set(TS_FILES 2_ru_RU.ts)

add_executable(2
  main.cpp
  ${TS_FILES}
)
target_link_libraries(2 Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql)

install(TARGETS 2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
