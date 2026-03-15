// Taktflow Systems - BCM UDS diagnostic system implementation

#include "systems/UdsSystem.h"

#include <busid/BusId.h>
#include <lifecycle/LifecycleManager.h>
#include <transport/ITransportSystem.h>
#include <transport/TransportConfiguration.h>
#include <uds/base/AbstractDiagJob.h>

namespace taktflow
{
namespace bcm
{
namespace systems
{

static uint8_t const VIN_DATA[] = "TAKTFLOW000000001";
static uint8_t const SW_VERSION_DATA[] = {0x01, 0x00, 0x00};
static uint8_t const HW_VERSION_DATA[] = {0x52, 0x45, 0x56, 0x2D, 0x41}; // "REV-A"

UdsSystem::UdsSystem(
    ::lifecycle::LifecycleManager& lManager,
    ::transport::ITransportSystem& transportSystem,
    signals::BcmSignals& signals,
    ::async::ContextType context,
    uint16_t udsAddress)
: AsyncLifecycleComponent()
, _udsLifecycleConnector(lManager)
, _transportSystem(transportSystem)
, _jobRoot()
, _diagnosticSessionControl(_udsLifecycleConnector, context, _dummySessionPersistence)
, _udsConfiguration{
      udsAddress,
      ::transport::TransportConfiguration::FUNCTIONAL_ALL_ISO14229,
      ::transport::TransportConfiguration::DIAG_PAYLOAD_SIZE,
      ::busid::SELFDIAG,
      true,
      false,
      true,
      context}
, _udsDispatcher(
      _connectionPool, _sendJobQueue, _udsConfiguration, _diagnosticSessionControl, _jobRoot)
, _readDataByIdentifier()
, _testerPresent()
, _readVin(0xF190u, VIN_DATA, 17u)
, _readSwVersion(0xF195u, SW_VERSION_DATA, 3u)
, _readHwVersion(0xF191u, HW_VERSION_DATA, 5u)
, _signals(signals)
, _context(context)
, _timeout()
{
    setTransitionContext(_context);
}

void UdsSystem::init()
{
    (void)_udsDispatcher.init();
    ::uds::AbstractDiagJob::setDefaultDiagSessionManager(_diagnosticSessionControl);
    _diagnosticSessionControl.setDiagDispatcher(&_udsDispatcher);
    _transportSystem.addTransportLayer(_udsDispatcher);
    addDiagJobs();
    transitionDone();
}

void UdsSystem::run()
{
    ::async::scheduleAtFixedRate(_context, *this, _timeout, 10, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void UdsSystem::shutdown()
{
    removeDiagJobs();
    _diagnosticSessionControl.setDiagDispatcher(nullptr);
    _diagnosticSessionControl.shutdown();
    _transportSystem.removeTransportLayer(_udsDispatcher);
    (void)_udsDispatcher.shutdown(
        ::transport::AbstractTransportLayer::ShutdownDelegate::
            create<UdsSystem, &UdsSystem::shutdownComplete>(*this));
}

void UdsSystem::shutdownComplete(::transport::AbstractTransportLayer&)
{
    _timeout.cancel();
    transitionDone();
}

void UdsSystem::addDiagJobs()
{
    (void)_jobRoot.addAbstractDiagJob(_readDataByIdentifier);
    (void)_jobRoot.addAbstractDiagJob(_readVin);
    (void)_jobRoot.addAbstractDiagJob(_readSwVersion);
    (void)_jobRoot.addAbstractDiagJob(_readHwVersion);
    (void)_jobRoot.addAbstractDiagJob(_testerPresent);
    (void)_jobRoot.addAbstractDiagJob(_diagnosticSessionControl);
}

void UdsSystem::removeDiagJobs()
{
    _jobRoot.removeAbstractDiagJob(_readDataByIdentifier);
    _jobRoot.removeAbstractDiagJob(_readVin);
    _jobRoot.removeAbstractDiagJob(_readSwVersion);
    _jobRoot.removeAbstractDiagJob(_readHwVersion);
    _jobRoot.removeAbstractDiagJob(_testerPresent);
    _jobRoot.removeAbstractDiagJob(_diagnosticSessionControl);
}

void UdsSystem::execute() {}

} // namespace systems
} // namespace bcm
} // namespace taktflow
