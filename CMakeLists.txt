cmake_minimum_required(VERSION 3.22)
project(astrogenerator
    VERSION 3.1.0
    LANGUAGES CXX
    )

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(
    -DVERSION=${CMAKE_PROJECT_VERSION}
    -DVERSION_STRING="${CMAKE_PROJECT_VERSION}"
)

add_subdirectory(src)

if(WIN32)
    find_program(WINDEPLOYQT windeployqt HINTS "${_qt_bin_dir}")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake.in" "${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake" @ONLY)
    set(CPACK_PRE_BUILD_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake)
endif()

include(CPack)
