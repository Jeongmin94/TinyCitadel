#pragma once

#include <memory>
#include <vector>

#include "Layer.h"

namespace Citadel
{

class LayerStack
{
public:
    LayerStack();
    ~LayerStack(); // 소멸 시 모든 레이어 Detach
    void Clear();  // 모든 레이어를 Detach하고 스택 비우기

public:
    void PushLayer(std::unique_ptr<Layer> layer);
    void PushOverlay(std::unique_ptr<Layer> overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

public:
    bool ValidateLayers();

    // 반복자 제공 (Application의 Run 루프에서 사용)
    std::vector<std::unique_ptr<Layer>>::iterator begin()
    {
        return m_Layers.begin();
    }
    std::vector<std::unique_ptr<Layer>>::iterator end()
    {
        return m_Layers.end();
    }
    std::vector<std::unique_ptr<Layer>>::reverse_iterator rbegin()
    {
        return m_Layers.rbegin();
    }
    std::vector<std::unique_ptr<Layer>>::reverse_iterator rend()
    {
        return m_Layers.rend();
    }

private:
    std::vector<std::unique_ptr<Layer>> m_Layers;

    // [0, m_LayerInsertIndex) : layer
    // [m_LayerInsertIndex, m_Layers.size()) : overlay
    uint32_t m_LayerInsertIndex = 0;
};

} // namespace Citadel
