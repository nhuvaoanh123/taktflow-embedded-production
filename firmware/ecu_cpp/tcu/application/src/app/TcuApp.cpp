// Taktflow Systems - TCU application (lifecycle manager, tasks, idle handler)

#include "app/TcuApp.h"
#include "systems/DoCanSystem.h"
#include "systems/TcuCanSystem.h"
#include "systems/TcuSystem.h"
#include "systems/TransportSystem.h"
#include "systems/UdsSystem.h"

#include <async/AsyncBinding.h>
#include <etl/alignment.h>
#include <etl/error_handler.h>
#include <lifecycle/LifecycleManager.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace app
{

using AsyncAdapter        = ::async::AsyncBinding::AdapterType;
using AsyncRuntimeMonitor = ::async::AsyncBinding::RuntimeMonitorType;
using AsyncContextHook    = ::async::AsyncBinding::ContextHookType;

static constexpr size_t MAX_NUM_COMPONENTS           = 12;
static constexpr size_t MAX_NUM_LEVELS               = 8;
static constexpr size_t MAX_NUM_COMPONENTS_PER_LEVEL = MAX_NUM_COMPONENTS;

using LifecycleManager = ::lifecycle::declare::
    LifecycleManager<MAX_NUM_COMPONENTS, MAX_NUM_LEVELS, MAX_NUM_COMPONENTS_PER_LEVEL>;

char const* const isrGroupNames[ISR_GROUP_COUNT] = {"can"};

AsyncRuntimeMonitor runtimeMonitor{
    AsyncContextHook::InstanceType::GetNameType::create<&AsyncAdapter::getTaskName>(),
    isrGroupNames};

LifecycleManager lifecycleManager{
    TASK_TCU,
    ::lifecycle::LifecycleManager::GetTimestampType::create<&getSystemTimeUs32Bit>()};

::etl::typed_storage<::taktflow::tcu::systems::TcuCanSystem> tcuCanSystem;
::etl::typed_storage<::taktflow::systems::TransportSystem> transportSystem;
::etl::typed_storage<::taktflow::tcu::systems::DoCanSystem> doCanSystem;
::etl::typed_storage<::taktflow::tcu::systems::TcuSystem> tcuSystem;
::etl::typed_storage<::taktflow::tcu::systems::UdsSystem> udsSystem;

class LifecycleMonitor : private ::lifecycle::ILifecycleListener
{
public:
    explicit LifecycleMonitor(LifecycleManager& manager) { manager.addLifecycleListener(*this); }
    bool isReadyForReset() const { return _isReadyForReset; }

private:
    void lifecycleLevelReached(
        uint8_t const level,
        ::lifecycle::ILifecycleComponent::Transition::Type const transition) override
    {
        if ((0 == level)
            && (transition == ::lifecycle::ILifecycleComponent::Transition::Type::SHUTDOWN))
        {
            _isReadyForReset = true;
        }
    }
    bool _isReadyForReset = false;
};

LifecycleMonitor lifecycleMonitor(lifecycleManager);

class IdleHandler : private ::async::RunnableType
{
public:
    void start() { ::async::execute(AsyncAdapter::TASK_IDLE, *this); }

private:
    void execute() override
    {
        if (lifecycleMonitor.isReadyForReset())
        {
            printf("TCU lifecycle shutdown complete\n");
            std::_Exit(0);
        }
        else
        {
            ::async::execute(AsyncAdapter::TASK_IDLE, *this);
        }
    }
};

IdleHandler idleHandler;

void startApp();

void etl_assert_function(etl::exception const& exception)
{
    std::cout << "Assertion at " << exception.file_name() << ':' << exception.line_number() << " ("
              << exception.what() << ") failed" << std::endl;
    std::abort();
}

void run()
{
    etl::set_assert_function(etl_assert_function);
    printf("TCU C++ (OpenBSW) starting...\n");
    AsyncAdapter::run(AsyncAdapter::StartAppFunctionType::create<&startApp>());
}

void startApp()
{
    // Level 3: CAN transceiver
    lifecycleManager.addComponent("can", tcuCanSystem.create(TASK_CAN), 3u);

    // Level 4: Transport message router
    lifecycleManager.addComponent("transport", transportSystem.create(TASK_UDS), 4u);

    // Level 5: DoCAN ISO-TP + TCU application SWCs
    lifecycleManager.addComponent(
        "docan",
        doCanSystem.create(
            *transportSystem, tcuCanSystem->getTransceiver(), TASK_UDS),
        5u);
    lifecycleManager.addComponent(
        "tcu", tcuSystem.create(TASK_TCU, tcuCanSystem->getTransceiver()), 5u);

    // Level 7: UDS diagnostic services
    lifecycleManager.addComponent(
        "uds",
        udsSystem.create(
            lifecycleManager,
            *transportSystem,
            tcuSystem->getSignals(),
            TASK_UDS,
            0x0015u),
        7u);

    lifecycleManager.transitionToLevel(MAX_NUM_LEVELS);

    runtimeMonitor.start();
    idleHandler.start();
}

using TimerTask = AsyncAdapter::TimerTask<1024 * 1>;
TimerTask timerTask{"timer"};

using IdleTask = AsyncAdapter::IdleTask<1024 * 2>;
IdleTask idleTask{"idle"};

using CanTask = AsyncAdapter::Task<TASK_CAN, 1024 * 2>;
CanTask canTask{"can"};

using TcuTask = AsyncAdapter::Task<TASK_TCU, 1024 * 4>;
TcuTask tcuTask{"tcu"};

using UdsTask = AsyncAdapter::Task<TASK_UDS, 1024 * 4>;
UdsTask udsTask{"uds"};

using BackgroundTask = AsyncAdapter::Task<TASK_BACKGROUND, 1024 * 2>;
BackgroundTask backgroundTask{"background"};

AsyncContextHook contextHook{runtimeMonitor};

} // namespace app

void app_main()
{
    ::app::run();
}
