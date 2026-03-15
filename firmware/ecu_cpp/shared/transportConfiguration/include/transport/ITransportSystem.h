// Taktflow Systems - Transport system interface (routing target management)

#pragma once

#include "transport/ITransportMessageProvider.h"

namespace transport
{
class AbstractTransportLayer;

class ITransportSystem
{
public:
    virtual void addTransportLayer(AbstractTransportLayer& layer)    = 0;
    virtual void removeTransportLayer(AbstractTransportLayer& layer) = 0;

    virtual ITransportMessageProvider& getTransportMessageProvider() = 0;
};

} // namespace transport
