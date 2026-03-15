add_executable(
    app.icu
    src/app/IcuApp.cpp
    src/signals/E2eProtection.cpp
    src/signals/CanFrameCodec.cpp
    src/systems/IcuCanSystem.cpp
    src/systems/IcuSystem.cpp
    src/systems/TransportSystem.cpp
    src/systems/DoCanSystem.cpp
    src/systems/UdsSystem.cpp
    src/swc/DashboardComponent.cpp
    src/swc/DtcDisplayComponent.cpp
)

set_target_properties(app.icu PROPERTIES SUFFIX ".elf")

target_include_directories(app.icu PRIVATE include)

target_link_libraries(
    app.icu
    PRIVATE asyncBinding
            etl
            etlImpl
            lifecycle
            cpp2can
            printf
            main
            osRtos
            asyncRtosImpl
            osHooks
            util
            runtime
            socketCanTransceiver
            bspMcu
            bspUart
            socBsp
            common
            uds
            udsConfigurationImpl
            docan
            transport
            transportRouterSimple
            loggerStub
)
