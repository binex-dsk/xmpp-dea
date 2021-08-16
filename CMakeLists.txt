cmake_minimum_required(VERSION 3.5)

project(xmpp-dea LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5Xml)
find_package(QXmpp REQUIRED)

add_executable(xmpp-dea
  main.cpp
)
target_link_libraries(xmpp-dea
    Qt5::Core
    Qt5::Xml
    qxmpp)

target_include_directories(xmpp-dea PRIVATE
    /usr/include/qxmpp)
