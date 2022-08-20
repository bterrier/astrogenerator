find_package(Qt6 REQUIRED COMPONENTS
    LinguistTools
    Sql
    Xml
)

add_library(libastrogenerator STATIC
    Calculastro.cpp Calculastro.h
    Constantes.h
    HauteurMaxTemps.cpp HauteurMaxTemps.h
    Objet.cpp Objet.h
    ObjetCP.cpp ObjetCP.h
    ObjetCPObs.cpp ObjetCPObs.h
    ObjetObs.cpp ObjetObs.h
    ObjetPlaneteObs.cpp ObjetPlaneteObs.h
    Soiree.cpp Soiree.h
    eyepiece.cpp eyepiece.h
    settings.cpp settings.h
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
    Qt::Sql
    Qt::Xml
)

install(FILES ${QM_FILES} DESTINATION translations)
