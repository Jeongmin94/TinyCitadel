#include "pch.h"

#include "LayerStack.h"

namespace Citadel
{

LayerStack::LayerStack() : m_LayerInsertIndex(0) {}

LayerStack::~LayerStack()
{
    // 소멸 전에 스택이 비어있지 않다면 정리
    if (m_Layers.empty() == false)
    {
        Clear();
    }
}

void LayerStack::Clear()
{
    std::cout << "Clearing LayerStack..." << std::endl;

    for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
    {
        if (*it != nullptr)
        {
            (*it)->OnDetach();
        }
    }
    m_Layers.clear(); // 벡터 비우기 (unique_ptr 소멸)
    m_LayerInsertIndex = 0;
    std::cout << "LayerStack Cleared." << std::endl;
}

void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
{
    auto it = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex,
                               std::move(layer));
    m_LayerInsertIndex++;
    (*it)->OnAttach();
}

void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
{
    m_Layers.emplace_back(std::move(overlay));
    m_Layers.back()->OnAttach();
}

// 주의: PopLayer/PopOverlay는 raw pointer를 받아서 제거
// 이는 외부에서 Layer* 를 가지고 있다가 제거 요청을 하는 경우를 위함.
// unique_ptr의 소유권 관리에 유의해야 함.
void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find_if(
        m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex,
        [&](const std::unique_ptr<Layer>& ptr) { return ptr.get() == layer; });

    if (it != m_Layers.begin() + m_LayerInsertIndex)
    {
        (*it)->OnDetach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Layer* overlay)
{
    auto it =
        std::find_if(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(),
                     [&](const std::unique_ptr<Layer>& ptr)
                     { return ptr.get() == overlay; });

    if (it != m_Layers.end())
    {
        (*it)->OnDetach();
        m_Layers.erase(it);
    }
}

bool LayerStack::ValidateLayers() { return true; }

} // namespace Citadel
