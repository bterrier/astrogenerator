find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(astrocalc STATIC
    astrocalc.cpp astrocalc.h
)

target_include_directories(astrocalc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(astrocalc PUBLIC
    Qt::Core
)
