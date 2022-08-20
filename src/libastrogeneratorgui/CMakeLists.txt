find_package(Qt6 REQUIRED COMPONENTS
    LinguistTools
    Widgets Sql Xml PrintSupport)

add_library(libastrogeneratorgui STATIC
    ActionsFenetre.cpp ActionsFenetre.h
    Carteciel.cpp Carteciel.h
    Diaporama.cpp Diaporama.h
    FenCreerSoiree.cpp FenCreerSoiree.h
    FenetreBDD.cpp FenetreBDD.h
    FenPersonaliser.cpp FenPersonaliser.h
    FenInfosCreation.cpp FenInfosCreation.h
    FenPreferences.cpp FenPreferences.h
    FenPrincipal.cpp FenPrincipal.h
    Interface.cpp Interface.h
    InterfaceCreation.cpp InterfaceCreation.h
    InterfaceLecture.cpp InterfaceLecture.h
    Onglet.cpp Onglet.h
    nightmodel.h nightmodel.cpp
    nightprinter.h nightprinter.cpp
    skymapdialog.cpp skymapdialog.h
    skymapdialog.ui
    aboutdialog.ui
    eyepiecesettingwidget.ui
    homewidget.ui
    aboutdialog.cpp aboutdialog.h
    eyepiecesettingwidget.cpp eyepiecesettingwidget.h
    homewidget.cpp homewidget.h
)

qt6_add_lupdate(libastrogeneratorgui TS_FILES
    astroGenerator_en.ts
    astroGenerator_fr.ts
)
qt6_add_lrelease(libastrogeneratorgui
    TS_FILES
        astroGenerator_en.ts
        astroGenerator_fr.ts
    QM_FILES_OUTPUT_VARIABLE
        QM_FILES
)

target_include_directories(libastrogeneratorgui
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libastrogeneratorgui PUBLIC
    astrocalc
    genericwidgets
    libastrogenerator
    Qt::PrintSupport
    Qt::Sql
    Qt::Widgets
    Qt::Xml
)

install(FILES ${QM_FILES} DESTINATION translations)
