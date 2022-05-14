find_package(Qt6 REQUIRED COMPONENTS
    LinguistTools
    Widgets Sql Xml PrintSupport)

add_library(libastrogenerator STATIC
    ActionsFenetre.cpp ActionsFenetre.h
    Calculastro.cpp Calculastro.h
    Carteciel.cpp Carteciel.h
    Diaporama.cpp Diaporama.h
    FenCreerSoiree.cpp FenCreerSoiree.h
    FenetreBDD.cpp FenetreBDD.h
    FenInfosCreation.cpp FenInfosCreation.h
    FenPersonaliser.cpp FenPersonaliser.h
    FenPreferences.cpp FenPreferences.h
    FenPrincipal.cpp FenPrincipal.h
    HauteurMaxTemps.cpp HauteurMaxTemps.h
    Interface.cpp Interface.h
    InterfaceCreation.cpp InterfaceCreation.h
    InterfaceLecture.cpp InterfaceLecture.h
    Objet.cpp Objet.h
    ObjetCP.cpp ObjetCP.h
    ObjetCPObs.cpp ObjetCPObs.h
    ObjetObs.cpp ObjetObs.h
    ObjetPlaneteObs.cpp ObjetPlaneteObs.h
    Onglet.cpp Onglet.h
    Soiree.cpp Soiree.h
    aboutdialog.cpp aboutdialog.h
    eyepiece.cpp eyepiece.h
    eyepiecesettingwidget.cpp eyepiecesettingwidget.h
    homewidget.cpp homewidget.h
    settings.cpp settings.h
    skymapdialog.cpp skymapdialog.h
    aboutdialog.ui
    eyepiecesettingwidget.ui
    homewidget.ui
    skymapdialog.ui
)

qt6_add_lupdate(libastrogenerator TS_FILES
    astroGenerator_en.ts
    astroGenerator_fr.ts
)
qt6_add_lrelease(libastrogenerator
    TS_FILES
        astroGenerator_en.ts
        astroGenerator_fr.ts
    QM_FILES_OUTPUT_VARIABLE
        QM_FILES
)

target_include_directories(libastrogenerator
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libastrogenerator PUBLIC
    astrocalc
    genericwidgets
    Qt::PrintSupport
    Qt::Sql
    Qt::Widgets
    Qt::Xml
)

install(FILES ${QM_FILES} DESTINATION translations)
