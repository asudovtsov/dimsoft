cmake_minimum_required(VERSION 3.20)

project(dimsoft-effect LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(DIMSOFT_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
if(CMAKE_LIBRARY_ARCHITECTURE AND CMAKE_INSTALL_LIBDIR STREQUAL "lib")
    set(DIMSOFT_INSTALL_LIBDIR "lib/${CMAKE_LIBRARY_ARCHITECTURE}")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core DBus Gui Quick Widgets)
find_package(KF6Config REQUIRED)
find_package(KF6CoreAddons REQUIRED)
find_package(KWin REQUIRED)

add_library(dimsoft MODULE
    src/dimsofteffect.cpp
    src/main.cpp
)

set_target_properties(dimsoft PROPERTIES
    PREFIX ""
)

target_link_libraries(dimsoft
    PRIVATE
        KWin::kwin
        KF6::ConfigCore
        KF6::CoreAddons
        Qt6::Core
)

install(TARGETS dimsoft DESTINATION "${DIMSOFT_INSTALL_LIBDIR}/qt6/plugins/kwin/effects/plugins")
