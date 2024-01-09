cmake_minimum_required(VERSION 3.22)
project(astrogenerator
    VERSION 3.1.0
    LANGUAGES CXX
    )

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(
    -DVERSION=${CMAKE_PROJECT_VERSION}
    -DVERSION_STRING="${CMAKE_PROJECT_VERSION}"
)

add_subdirectory(src)
add_subdirectory(tests)

if(WIN32)
    find_program(WINDEPLOYQT windeployqt HINTS "${_qt_bin_dir}")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake.in" "${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake" @ONLY)
    set(CPACK_PRE_BUILD_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake)

    set(CPACK_PACKAGE_INSTALL_DIRECTORY "AstroGenerator")
    set(CPACK_NSIS_DISPLAY_NAME "AstroGenerator ${PROJECT_VERSION_MAJOR}")
    set(CPACK_NSIS_PACKAGE_NAME "AstroGenerator ${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VENDOR "Benjamin Terrier")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_IGNORE_LICENSE_PAGE ON)
    set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_SOURCE_DIR}/logoastrogenerator.ico)
    set(CPACK_NSIS_INSTALLED_ICON_NAME "astroGenerator.exe")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/bterrier/astrogenerator")
    set(CPACK_NSIS_MANIFEST_DPI_AWARE ON)
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\AstroGenerator ${PROJECT_VERSION_MAJOR}.lnk' '$INSTDIR\\\\astroGenerator.exe' "
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$MUI_TEMP\\\\AstroGenerator ${PROJECT_VERSION_MAJOR}.lnk'"
    )
endif()

include(CPack)
