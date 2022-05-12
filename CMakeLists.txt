cmake_minimum_required(VERSION 3.22)
project(homework VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.12/5.12.12/mingw73_64/lib/cmake")
find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(homework main.cpp)
target_link_libraries(homework
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )