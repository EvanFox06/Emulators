cmake_minimum_required(VERSION 3.16)

project(Emulators VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup()

qt_add_executable(Emulators
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    emulators.cpp
    emulators.h
    emulator.cpp
    emulator.h
)

target_link_libraries(Emulators PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Emulators PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(Emulators PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Emulators
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
