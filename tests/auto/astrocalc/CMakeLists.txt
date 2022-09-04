find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(tst_astrocalc
    tst_astrocalc.cpp
)

target_link_libraries(tst_astrocalc PRIVATE Qt6::Test astrocalc)
