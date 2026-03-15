add_executable(
    app.tcu
    src/app/TcuApp.cpp
    src/signals/E2eProtection.cpp
    src/signals/CanFrameCodec.cpp
    src/systems/TcuCanSystem.cpp
    src/systems/TcuSystem.cpp
    src/systems/TransportSystem.cpp
    src/systems/DoCanSystem.cpp
    src/systems/UdsSystem.cpp
    src/swc/DtcStoreComponent.cpp
    src/swc/Obd2PidsComponent.cpp
)

set_target_properties(app.tcu PROPERTIES SUFFIX ".elf")

target_include_directories(app.tcu PRIVATE include)

target_link_libraries(
    app.tcu
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
