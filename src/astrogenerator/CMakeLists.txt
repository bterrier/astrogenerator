file(
    COPY
        ../../dbastrogenerator
        ../../doc.pdf
        ../../dessin-constellation.txt
        ../../icones
        ../../nom_constellation.txt
        ../../qlocale-pays.txt
        ../../VSOP87
    DESTINATION
        ${CMAKE_CURRENT_BINARY_DIR}
    )

add_executable(astroGenerator WIN32
    main.cpp
    ../../resources.qrc
    ../../astrogenerator.rc
)

target_link_libraries(astroGenerator PRIVATE
    libastrogenerator
    libastrogeneratorgui
)

install(TARGETS astroGenerator RUNTIME DESTINATION .)
install(
    FILES
        ../../dbastrogenerator
        ../../doc.pdf
        ../../dessin-constellation.txt
        ../../nom_constellation.txt
        ../../qlocale-pays.txt
    DESTINATION
        .
)

install(
    DIRECTORY
        ../../icones
        ../../VSOP87
    DESTINATION
        .
)

