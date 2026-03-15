add_subdirectory(bspConfiguration)

add_library(main main/src/main.cpp)

target_include_directories(main PUBLIC main/include)

target_link_libraries(
    main
    PRIVATE asyncBinding lifecycle)

add_library(freeRtosPort ALIAS freeRtosPosixPort)
add_library(freeRtosPortImpl ALIAS freeRtosPosix)

add_library(osHooks main/src/osHooks/osHooks.cpp)
target_link_libraries(osHooks PRIVATE freeRtos)

add_subdirectory(freeRtosCoreConfiguration)
