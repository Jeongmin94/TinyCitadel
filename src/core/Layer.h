#pragma once

#include <string>

namespace Citadel
{

// 모든 레이어(모듈)이 상속받아야 하는 기본 인터페이스
class Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

public:
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}

public:
    const std::string& GetName() const { return m_DebugName; }

private:
    std::string m_DebugName;
};

} // namespace Citadel