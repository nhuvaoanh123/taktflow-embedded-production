// Taktflow Systems - TCU UDS diagnostic system (OpenBSW uds module)

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <lifecycle/AsyncLifecycleComponent.h>
#include <uds/DiagDispatcher.h>
#include <uds/DummySessionPersistence.h>
#include <uds/UdsLifecycleConnector.h>
#include <uds/jobs/ReadIdentifierFromMemory.h>
#include <uds/services/readdata/ReadDataByIdentifier.h>
#include <uds/services/sessioncontrol/DiagnosticSessionControl.h>
#include <uds/services/testerpresent/TesterPresent.h>

namespace lifecycle
{
class LifecycleManager;
}

namespace transport
{
class ITransportSystem;
}

namespace taktflow
{
namespace tcu
{

namespace signals
{
struct TcuSignals;
}

namespace systems
{

class UdsSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    UdsSystem(
        ::lifecycle::LifecycleManager& lManager,
        ::transport::ITransportSystem& transportSystem,
        signals::TcuSignals& signals,
        ::async::ContextType context,
        uint16_t udsAddress);

    void init() override;
    void run() override;
    void shutdown() override;

private:
    void addDiagJobs();
    void removeDiagJobs();
    void shutdownComplete(::transport::AbstractTransportLayer&);
    void execute() override;

    ::uds::UdsLifecycleConnector _udsLifecycleConnector;
    ::transport::ITransportSystem& _transportSystem;
    ::uds::DummySessionPersistence _dummySessionPersistence;

    ::uds::DiagJobRoot _jobRoot;
    ::uds::DiagnosticSessionControl _diagnosticSessionControl;
    ::uds::DiagnosisConfiguration _udsConfiguration;
    ::etl::pool<::uds::IncomingDiagConnection, 5> _connectionPool;
    ::etl::queue<::uds::TransportJob, 16> _sendJobQueue;
    ::uds::DiagDispatcher _udsDispatcher;

    // Services
    ::uds::ReadDataByIdentifier _readDataByIdentifier;
    ::uds::TesterPresent _testerPresent;

    // Static DIDs (0x22)
    ::uds::ReadIdentifierFromMemory _readVin;
    ::uds::ReadIdentifierFromMemory _readSwVersion;
    ::uds::ReadIdentifierFromMemory _readHwVersion;

    signals::TcuSignals& _signals;
    ::async::ContextType _context;
    ::async::TimeoutType _timeout;
};

} // namespace systems
} // namespace tcu
} // namespace taktflow
