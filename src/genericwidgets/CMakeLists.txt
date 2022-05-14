find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(genericwidgets STATIC
    clockwidget.cpp clockwidget.h
    genericwidgets.cpp clockwidget.h
    timerwidget.cpp clockwidget.h
)

target_include_directories(genericwidgets
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(genericwidgets PUBLIC
    Qt::Widgets
)
