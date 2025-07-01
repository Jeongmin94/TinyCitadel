#include "pch.h"

#include "Layer.h"

namespace Citadel
{
Layer::Layer(const std::string& name) : m_DebugName(name) {}

Layer::~Layer()
{
    std::cout << "Layer " << m_DebugName << " destroyed" << std::endl;
}

} // namespace Citadel
