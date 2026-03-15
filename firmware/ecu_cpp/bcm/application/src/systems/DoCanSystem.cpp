// Taktflow Systems - BCM DoCAN transport layer implementation

#include "systems/DoCanSystem.h"

#include <bsp/timer/SystemTimer.h>
#include <busid/BusId.h>
#include <docan/datalink/DoCanFrameCodecConfigPresets.h>
#include <transport/ITransportSystem.h>

namespace
{
uint32_t const TIMEOUT_DOCAN_MS       = 10U;
uint16_t const ALLOCATE_TIMEOUT       = 1000U;
uint16_t const RX_TIMEOUT             = 1000U;
uint16_t const TX_CALLBACK_TIMEOUT    = 1000U;
uint16_t const FLOW_CONTROL_TIMEOUT   = 1000U;
uint8_t const ALLOCATE_RETRY_COUNT    = 15U;
uint8_t const FLOW_CONTROL_WAIT_COUNT = 15U;
uint16_t const MIN_SEPARATION_TIME    = 200U;
uint8_t const BLOCK_SIZE              = 15U;

uint32_t systemUs() { return getSystemTimeUs32Bit(); }

} // namespace

namespace taktflow
{
namespace bcm
{
namespace systems
{

// BCM diagnostic addressing: tester sends on 0x616, BCM responds on 0x61E
// Transport source = 0xF0 (tester), target = 0x16 (BCM logical address)
DoCanSystem::AddressingFilterType::AddressEntryType DoCanSystem::_addresses[]
    = {{0x616u, 0x61Eu, 0x00F0u, 0x0016u, 0, 0}};

DoCanSystem::DoCanSystem(
    ::transport::ITransportSystem& transportSystem,
    ::can::ICanTransceiver& canTransceiver,
    ::async::ContextType asyncContext)
: _context(asyncContext)
, _cyclicTimeout()
, _canTransceiver(canTransceiver)
, _transportSystem(transportSystem)
, _addressing()
, _frameSizeMapper()
, _classicCodec(::docan::DoCanFrameCodecConfigPresets::PADDED_CLASSIC, _frameSizeMapper)
, _classicAddressingFilter()
, _parameters(
      ::etl::delegate<decltype(systemUs)>::create<&systemUs>(),
      ALLOCATE_TIMEOUT,
      RX_TIMEOUT,
      TX_CALLBACK_TIMEOUT,
      FLOW_CONTROL_TIMEOUT,
      ALLOCATE_RETRY_COUNT,
      FLOW_CONTROL_WAIT_COUNT,
      MIN_SEPARATION_TIME,
      BLOCK_SIZE)
, _transportLayerConfig(_parameters)
, _physicalTransceiver(
      _canTransceiver,
      _classicAddressingFilter,
      _classicAddressingFilter,
      _addressing)
, _transportLayers()
, _tickGenerator(asyncContext, _transportLayers)
, _codecs{&_classicCodec}
{
    setTransitionContext(asyncContext);
}

void DoCanSystem::init()
{
    _classicAddressingFilter.init(::etl::make_span(_addresses), ::etl::make_span(_codecs));

    _transportLayers.emplace_back(
        ::busid::CAN_0,
        ::etl::ref(_context),
        ::etl::ref(_classicAddressingFilter),
        ::etl::ref(_physicalTransceiver),
        ::etl::ref(_tickGenerator),
        ::etl::ref(_transportLayerConfig),
        0u);

    transitionDone();
}

void DoCanSystem::run()
{
    for (auto& layer : _transportLayers.getTransportLayers())
    {
        _transportSystem.addTransportLayer(layer);
    }
    _transportLayers.init();

    ::async::scheduleAtFixedRate(
        _context, *this, _cyclicTimeout, TIMEOUT_DOCAN_MS, ::async::TimeUnit::MILLISECONDS);

    transitionDone();
}

void DoCanSystem::shutdown()
{
    _cyclicTimeout.cancel();
    for (auto& layer : _transportLayers.getTransportLayers())
    {
        _transportSystem.removeTransportLayer(layer);
    }
    transitionDone();
}

void DoCanSystem::execute() { _transportLayers.cyclicTask(systemUs()); }

// TickGeneratorAdapter

DoCanSystem::TickGeneratorAdapter::TickGeneratorAdapter(
    ::async::ContextType const context, TransportLayers& layers)
: _layers(layers), _context(context)
{}

void DoCanSystem::TickGeneratorAdapter::cancelTimeout() { _tickTimeout.cancel(); }

void DoCanSystem::TickGeneratorAdapter::tickNeeded()
{
    ::async::schedule(_context, *this, _tickTimeout, 200U, ::async::TimeUnit::MICROSECONDS);
}

void DoCanSystem::TickGeneratorAdapter::execute()
{
    if (_layers.tick(systemUs()))
    {
        ::async::schedule(_context, *this, _tickTimeout, 200U, ::async::TimeUnit::MICROSECONDS);
    }
}

} // namespace systems
} // namespace bcm
} // namespace taktflow
