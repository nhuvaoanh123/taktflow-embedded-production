add_executable(
    app.bcm
    src/app/BcmApp.cpp
    src/signals/E2eProtection.cpp
    src/signals/CanFrameCodec.cpp
    src/systems/BcmCanSystem.cpp
    src/systems/BcmSystem.cpp
    src/systems/TransportSystem.cpp
    src/systems/DoCanSystem.cpp
    src/systems/UdsSystem.cpp
    src/swc/LightsComponent.cpp
    src/swc/IndicatorsComponent.cpp
    src/swc/DoorLockComponent.cpp
)

set_target_properties(app.bcm PROPERTIES SUFFIX ".elf")

target_include_directories(app.bcm PRIVATE include)

target_link_libraries(
    app.bcm
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
