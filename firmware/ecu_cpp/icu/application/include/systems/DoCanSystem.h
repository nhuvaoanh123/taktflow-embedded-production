// Taktflow Systems - ICU DoCAN transport layer lifecycle component

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <docan/addressing/DoCanNormalAddressing.h>
#include <docan/addressing/DoCanNormalAddressingFilter.h>
#include <docan/can/DoCanPhysicalCanTransceiver.h>
#include <docan/datalink/DoCanDefaultFrameSizeMapper.h>
#include <docan/datalink/DoCanFdFrameSizeMapper.h>
#include <docan/datalink/DoCanFrameCodec.h>
#include <docan/transmitter/IDoCanTickGenerator.h>
#include <docan/transport/DoCanTransportLayerContainer.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace can
{
class ICanTransceiver;
}

namespace transport
{
class ITransportSystem;
}

namespace taktflow
{
namespace icu
{
namespace systems
{

class DoCanSystem final
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    using AddressingType    = ::docan::DoCanNormalAddressing<>;
    using DataLinkLayerType = AddressingType::DataLinkLayerType;
    using FrameCodecType    = ::docan::DoCanFrameCodec<DataLinkLayerType>;
    using AddressingFilterType = ::docan::DoCanNormalAddressingFilter<DataLinkLayerType>;

    DoCanSystem(
        ::transport::ITransportSystem& transportSystem,
        ::can::ICanTransceiver& canTransceiver,
        ::async::ContextType asyncContext);

    void init() final;
    void run() final;
    void shutdown() final;

private:
    using TransportLayers
        = ::docan::declare::DoCanTransportLayerContainer<DataLinkLayerType, 1>;

    class TickGeneratorAdapter final
    : public ::docan::IDoCanTickGenerator
    , private ::async::RunnableType
    {
    public:
        TickGeneratorAdapter(::async::ContextType context, TransportLayers& layers);
        void cancelTimeout();

    private:
        void execute() final;
        void tickNeeded() final;

        TransportLayers& _layers;
        ::async::TimeoutType _tickTimeout;
        ::async::ContextType _context;
    };

    void execute() final;

    ::async::ContextType const _context;
    ::async::TimeoutType _cyclicTimeout;

    ::can::ICanTransceiver& _canTransceiver;
    ::transport::ITransportSystem& _transportSystem;

    AddressingType _addressing;
    ::docan::DoCanFdFrameSizeMapper<DataLinkLayerType::FrameSizeType> _frameSizeMapper;
    FrameCodecType _classicCodec;
    AddressingFilterType _classicAddressingFilter;

    ::docan::DoCanParameters _parameters;
    ::docan::declare::DoCanTransportLayerConfig<DataLinkLayerType, 80U, 15U, 64U>
        _transportLayerConfig;
    ::docan::DoCanPhysicalCanTransceiver<AddressingType> _physicalTransceiver;
    TransportLayers _transportLayers;
    TickGeneratorAdapter _tickGenerator;

    FrameCodecType const* _codecs[1];

    static AddressingFilterType::AddressEntryType _addresses[];
};

} // namespace systems
} // namespace icu
} // namespace taktflow
