# PSO 시스템 단계적 구현 가이드

## 현재 프로젝트 상태 분석

### 기존 구조
```cpp
// 현재 상태
class Renderer
{
private:
    Mesh m_Mesh;                    // 단일 메시
    ShapeRenderer m_ShapeRenderer;  // 단일 렌더러
    
public:
    void Render()
    {
        // 하드코딩된 단일 객체 렌더링
        m_ShapeRenderer.Draw(m_Mesh, {0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f});
    }
};
```

### 목표
- 기존 코드를 최대한 보존하면서 점진적 개선
- 각 단계가 독립적으로 동작 가능
- 언제든지 이전 단계로 되돌아갈 수 있음

---

## 1단계: 다중 객체 렌더링 (난이도: ⭐)

### 목표
- 여러 개의 객체를 렌더링할 수 있게 하기
- 기존 ShapeRenderer 그대로 사용

### 구현

#### 1.1 RenderItem 구조체 추가
```cpp
// src/renderer/Renderer.h에 추가
struct RenderItem
{
    Mesh mesh;
    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 size = {1.0f, 1.0f};
    float rotation = 0.0f;
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};
```

#### 1.2 Renderer 클래스 수정
```cpp
class Renderer
{
private:
    std::vector<RenderItem> m_RenderItems;  // 추가
    ShapeRenderer m_ShapeRenderer;
    
public:
    // 기존 메서드 유지
    void Render()
    {
        // 모든 아이템 렌더링
        for (const auto& item : m_RenderItems)
        {
            m_ShapeRenderer.Draw(item.mesh, item.position, item.size, 
                               item.rotation, item.color);
        }
    }
    
    // 새로운 메서드 추가
    void AddRenderItem(const RenderItem& item)
    {
        m_RenderItems.push_back(item);
    }
    
    void ClearRenderItems()
    {
        m_RenderItems.clear();
    }
};
```

#### 1.3 사용 예시
```cpp
// main.cpp 또는 Application.cpp에서
void SetupScene()
{
    // 여러 도형 추가
    RenderItem redCircle;
    redCircle.mesh = ShapeFactory::CreateCircle();
    redCircle.position = {-0.5f, 0.0f};
    redCircle.color = {1.0f, 0.0f, 0.0f, 1.0f};
    renderer.AddRenderItem(redCircle);
    
    RenderItem blueTriangle;
    blueTriangle.mesh = ShapeFactory::CreateTriangle();
    blueTriangle.position = {0.5f, 0.0f};
    blueTriangle.color = {0.0f, 0.0f, 1.0f, 1.0f};
    renderer.AddRenderItem(blueTriangle);
}
```

### 테스트 방법
1. 삼각형과 원을 동시에 화면에 표시
2. 각각 다른 위치와 색상으로 렌더링
3. 기존 코드와 동일한 성능 확인

---

## 2단계: 간단한 재질 시스템 (난이도: ⭐⭐)

### 목표
- 색상 기반의 간단한 재질 시스템 구축
- 같은 색상끼리 그룹화 가능하게 만들기

### 구현

#### 2.1 SimpleMaterial 클래스 생성
```cpp
// src/renderer/SimpleMaterial.h
class SimpleMaterial
{
private:
    glm::vec4 m_Color;
    std::string m_Name;
    
public:
    SimpleMaterial(const glm::vec4& color, const std::string& name = "")
        : m_Color(color), m_Name(name) {}
    
    const glm::vec4& GetColor() const { return m_Color; }
    const std::string& GetName() const { return m_Name; }
    
    // 재질 비교용 ID (색상 기반)
    uint32_t GetID() const
    {
        uint32_t r = static_cast<uint32_t>(m_Color.r * 255);
        uint32_t g = static_cast<uint32_t>(m_Color.g * 255);
        uint32_t b = static_cast<uint32_t>(m_Color.b * 255);
        uint32_t a = static_cast<uint32_t>(m_Color.a * 255);
        return (r << 24) | (g << 16) | (b << 8) | a;
    }
};
```

#### 2.2 RenderItem 수정
```cpp
struct RenderItem
{
    Mesh mesh;
    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 size = {1.0f, 1.0f};
    float rotation = 0.0f;
    SimpleMaterial* material = nullptr;  // 변경: 재질 포인터 사용
    
    // 이전 버전과의 호환성을 위한 생성자
    RenderItem(const Mesh& m, const glm::vec2& pos, const glm::vec2& s, 
               float rot, const glm::vec4& color)
        : mesh(m), position(pos), size(s), rotation(rot)
    {
        static SimpleMaterial defaultMaterial(color);
        material = &defaultMaterial;
    }
};
```

#### 2.3 MaterialManager 클래스 생성
```cpp
// src/renderer/MaterialManager.h
class MaterialManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<SimpleMaterial>> m_Materials;
    
public:
    SimpleMaterial* CreateMaterial(const std::string& name, const glm::vec4& color)
    {
        auto material = std::make_unique<SimpleMaterial>(color, name);
        SimpleMaterial* ptr = material.get();
        m_Materials[name] = std::move(material);
        return ptr;
    }
    
    SimpleMaterial* GetMaterial(const std::string& name)
    {
        auto it = m_Materials.find(name);
        return (it != m_Materials.end()) ? it->second.get() : nullptr;
    }
    
    void CreateDefaultMaterials()
    {
        CreateMaterial("Red", {1.0f, 0.0f, 0.0f, 1.0f});
        CreateMaterial("Green", {0.0f, 1.0f, 0.0f, 1.0f});
        CreateMaterial("Blue", {0.0f, 0.0f, 1.0f, 1.0f});
        CreateMaterial("White", {1.0f, 1.0f, 1.0f, 1.0f});
        CreateMaterial("Yellow", {1.0f, 1.0f, 0.0f, 1.0f});
    }
};
```

#### 2.4 사용 예시
```cpp
// Renderer 클래스에 MaterialManager 추가
class Renderer
{
private:
    MaterialManager m_MaterialManager;
    std::vector<RenderItem> m_RenderItems;
    
public:
    bool Init(const Window* window)
    {
        // 기존 초기화 코드...
        
        // 기본 재질 생성
        m_MaterialManager.CreateDefaultMaterials();
        
        return true;
    }
    
    SimpleMaterial* GetMaterial(const std::string& name)
    {
        return m_MaterialManager.GetMaterial(name);
    }
};

// 사용법
void SetupScene()
{
    RenderItem redCircle;
    redCircle.mesh = ShapeFactory::CreateCircle();
    redCircle.position = {-0.5f, 0.0f};
    redCircle.material = renderer.GetMaterial("Red");
    renderer.AddRenderItem(redCircle);
}
```

### 테스트 방법
1. 이름으로 재질 생성 및 사용
2. 같은 재질을 여러 객체에 적용
3. 재질 변경 시 모든 객체에 반영되는지 확인

---

## 3단계: 렌더링 커맨드 시스템 (난이도: ⭐⭐⭐)

### 목표
- 렌더링 명령을 즉시 실행하지 않고 수집
- 나중에 한 번에 실행하는 시스템 구축

### 구현

#### 3.1 RenderCommand 구조체 생성
```cpp
// src/renderer/RenderCommand.h
struct RenderCommand
{
    Mesh mesh;
    SimpleMaterial* material;
    glm::mat4 modelMatrix;
    
    // Transform 정보로부터 모델 매트릭스 생성
    static RenderCommand Create(const Mesh& mesh, SimpleMaterial* material,
                               const glm::vec2& position, const glm::vec2& size, 
                               float rotation)
    {
        RenderCommand cmd;
        cmd.mesh = mesh;
        cmd.material = material;
        
        // 모델 매트릭스 계산
        cmd.modelMatrix = glm::mat4(1.0f);
        cmd.modelMatrix = glm::translate(cmd.modelMatrix, glm::vec3(position, 0.0f));
        cmd.modelMatrix = glm::rotate(cmd.modelMatrix, glm::radians(rotation), 
                                     glm::vec3(0.0f, 0.0f, 1.0f));
        cmd.modelMatrix = glm::scale(cmd.modelMatrix, glm::vec3(size, 1.0f));
        
        return cmd;
    }
};
```

#### 3.2 CommandBuffer 클래스 생성
```cpp
// src/renderer/CommandBuffer.h
class CommandBuffer
{
private:
    std::vector<RenderCommand> m_Commands;
    
public:
    void AddCommand(const RenderCommand& command)
    {
        m_Commands.push_back(command);
    }
    
    void Execute(ShapeRenderer& renderer)
    {
        for (const auto& cmd : m_Commands)
        {
            // 모델 매트릭스에서 변환 정보 추출 (간단한 방법)
            glm::vec3 position = glm::vec3(cmd.modelMatrix[3]);
            glm::vec2 pos2d = glm::vec2(position.x, position.y);
            
            // 스케일 추출 (간단한 방법)
            glm::vec2 scale = glm::vec2(
                glm::length(glm::vec3(cmd.modelMatrix[0])),
                glm::length(glm::vec3(cmd.modelMatrix[1]))
            );
            
            // 회전 추출 (간단화)
            float rotation = 0.0f; // 일단 0으로 설정
            
            renderer.Draw(cmd.mesh, pos2d, scale, rotation, cmd.material->GetColor());
        }
    }
    
    void Clear()
    {
        m_Commands.clear();
    }
    
    size_t GetCommandCount() const
    {
        return m_Commands.size();
    }
};
```

#### 3.3 Renderer 클래스 수정
```cpp
class Renderer
{
private:
    CommandBuffer m_CommandBuffer;  // 추가
    MaterialManager m_MaterialManager;
    ShapeRenderer m_ShapeRenderer;
    
public:
    void Submit(const Mesh& mesh, SimpleMaterial* material,
                const glm::vec2& position, const glm::vec2& size = {1.0f, 1.0f},
                float rotation = 0.0f)
    {
        RenderCommand cmd = RenderCommand::Create(mesh, material, position, size, rotation);
        m_CommandBuffer.AddCommand(cmd);
    }
    
    void Render()
    {
        // 모든 명령 실행
        m_CommandBuffer.Execute(m_ShapeRenderer);
        
        // 명령 버퍼 정리
        m_CommandBuffer.Clear();
    }
    
    // 기존 메서드들도 유지 (호환성)
    void AddRenderItem(const RenderItem& item)
    {
        Submit(item.mesh, item.material, item.position, item.size, item.rotation);
    }
};
```

#### 3.4 사용 예시
```cpp
void GameLoop()
{
    // 렌더링 명령 제출
    renderer.Submit(circleMesh, renderer.GetMaterial("Red"), {-0.5f, 0.0f});
    renderer.Submit(triangleMesh, renderer.GetMaterial("Blue"), {0.5f, 0.0f});
    renderer.Submit(circleMesh, renderer.GetMaterial("Red"), {0.0f, 0.5f});
    
    // 한 번에 실행
    renderer.Render();
}
```

### 테스트 방법
1. 여러 렌더링 명령을 제출
2. Render() 호출 시 모든 명령이 실행되는지 확인
3. 명령 수집과 실행이 분리되어 있는지 확인

---

## 4단계: 간단한 정렬 시스템 (난이도: ⭐⭐⭐⭐)

### 목표
- 재질별로 렌더링 명령을 정렬
- 같은 재질끼리 묶어서 렌더링

### 구현

#### 4.1 RenderCommand에 정렬 키 추가
```cpp
struct RenderCommand
{
    Mesh mesh;
    SimpleMaterial* material;
    glm::mat4 modelMatrix;
    
    // 정렬을 위한 키 생성
    uint64_t GetSortKey() const
    {
        if (!material) return 0;
        
        uint64_t materialID = material->GetID();
        return materialID;
    }
    
    // 정렬 비교 연산자
    bool operator<(const RenderCommand& other) const
    {
        return GetSortKey() < other.GetSortKey();
    }
};
```

#### 4.2 CommandBuffer에 정렬 기능 추가
```cpp
class CommandBuffer
{
private:
    std::vector<RenderCommand> m_Commands;
    
public:
    void Sort()
    {
        std::sort(m_Commands.begin(), m_Commands.end());
    }
    
    void Execute(ShapeRenderer& renderer)
    {
        // 실행 전 정렬
        Sort();
        
        SimpleMaterial* currentMaterial = nullptr;
        
        for (const auto& cmd : m_Commands)
        {
            // 재질 변경 추적 (디버깅용)
            if (currentMaterial != cmd.material)
            {
                currentMaterial = cmd.material;
                // 콘솔에 재질 변경 로그 출력
                std::cout << "Material changed to: " << currentMaterial->GetName() << std::endl;
            }
            
            // 렌더링 실행
            glm::vec3 position = glm::vec3(cmd.modelMatrix[3]);
            glm::vec2 pos2d = glm::vec2(position.x, position.y);
            
            glm::vec2 scale = glm::vec2(
                glm::length(glm::vec3(cmd.modelMatrix[0])),
                glm::length(glm::vec3(cmd.modelMatrix[1]))
            );
            
            renderer.Draw(cmd.mesh, pos2d, scale, 0.0f, cmd.material->GetColor());
        }
    }
};
```

#### 4.3 성능 측정 도구 추가
```cpp
class PerformanceTracker
{
private:
    std::unordered_map<std::string, int> m_Counters;
    
public:
    void Reset()
    {
        m_Counters.clear();
    }
    
    void Increment(const std::string& counterName)
    {
        m_Counters[counterName]++;
    }
    
    void PrintReport()
    {
        std::cout << "=== Performance Report ===" << std::endl;
        for (const auto& [name, count] : m_Counters)
        {
            std::cout << name << ": " << count << std::endl;
        }
        std::cout << "=========================" << std::endl;
    }
};
```

#### 4.4 사용 예시
```cpp
void TestSorting()
{
    // 의도적으로 섞인 순서로 제출
    renderer.Submit(circleMesh, renderer.GetMaterial("Red"), {-0.5f, 0.0f});
    renderer.Submit(triangleMesh, renderer.GetMaterial("Blue"), {0.5f, 0.0f});
    renderer.Submit(circleMesh, renderer.GetMaterial("Red"), {0.0f, 0.5f});
    renderer.Submit(triangleMesh, renderer.GetMaterial("Blue"), {0.0f, -0.5f});
    renderer.Submit(circleMesh, renderer.GetMaterial("Red"), {0.5f, 0.5f});
    
    // 렌더링 (자동으로 정렬됨)
    renderer.Render();
    
    // 출력 예상:
    // Material changed to: Red
    // Material changed to: Blue
    // (Red 객체 3개 → Blue 객체 2개 순서로 렌더링)
}
```

### 테스트 방법
1. 다양한 재질의 객체를 무작위 순서로 제출
2. 실제로 재질별로 정렬되어 렌더링되는지 확인
3. 재질 변경 횟수가 최소화되는지 로그로 확인

---

## 5단계: 상태 관리 최적화 (난이도: ⭐⭐⭐⭐⭐)

### 목표
- OpenGL 상태 변경을 최소화
- 불필요한 상태 설정 제거

### 구현

#### 5.1 간단한 RenderState 구조체
```cpp
// src/renderer/RenderState.h
struct RenderState
{
    GLuint shaderProgram = 0;
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    
    bool operator==(const RenderState& other) const
    {
        return shaderProgram == other.shaderProgram && 
               color == other.color;
    }
    
    bool operator!=(const RenderState& other) const
    {
        return !(*this == other);
    }
};
```

#### 5.2 StateManager 클래스 생성
```cpp
// src/renderer/StateManager.h
class StateManager
{
private:
    RenderState m_CurrentState;
    int m_StateChanges = 0;
    
public:
    void SetState(const RenderState& state)
    {
        if (m_CurrentState != state)
        {
            if (m_CurrentState.shaderProgram != state.shaderProgram)
            {
                glUseProgram(state.shaderProgram);
                m_StateChanges++;
            }
            
            m_CurrentState = state;
        }
    }
    
    void ResetCounters()
    {
        m_StateChanges = 0;
    }
    
    int GetStateChanges() const
    {
        return m_StateChanges;
    }
};
```

#### 5.3 ShapeRenderer 수정
```cpp
class ShapeRenderer
{
private:
    StateManager m_StateManager;  // 추가
    
public:
    void BeginBatch()
    {
        m_StateManager.ResetCounters();
    }
    
    void EndBatch()
    {
        std::cout << "State changes: " << m_StateManager.GetStateChanges() << std::endl;
    }
    
    void Draw(const Mesh& mesh, const glm::vec2& position,
              const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        // 상태 설정
        RenderState state;
        state.shaderProgram = m_ShaderProgramID;
        state.color = color;
        
        m_StateManager.SetState(state);
        
        // 기존 렌더링 코드...
    }
};
```

### 테스트 방법
1. 동일한 재질의 객체들이 연속으로 렌더링될 때 상태 변경 횟수 확인
2. 정렬 전후의 상태 변경 횟수 비교
3. 성능 향상 측정

---

## 6단계: 완전한 통합 (난이도: ⭐⭐⭐⭐⭐)

### 목표
- 모든 시스템을 통합하여 완전한 PSO 시스템 구축
- 기존 인터페이스와의 호환성 유지

### 구현

#### 6.1 최종 Renderer 클래스
```cpp
class Renderer
{
private:
    // 핵심 시스템들
    CommandBuffer m_CommandBuffer;
    MaterialManager m_MaterialManager;
    ShapeRenderer m_ShapeRenderer;
    StateManager m_StateManager;
    
    // 성능 추적
    PerformanceTracker m_PerformanceTracker;
    
public:
    bool Init(const Window* window)
    {
        // 기존 초기화...
        m_MaterialManager.CreateDefaultMaterials();
        return true;
    }
    
    // 새로운 API
    void Submit(const Mesh& mesh, const std::string& materialName,
                const glm::vec2& position, const glm::vec2& size = {1.0f, 1.0f},
                float rotation = 0.0f)
    {
        SimpleMaterial* material = m_MaterialManager.GetMaterial(materialName);
        if (material)
        {
            RenderCommand cmd = RenderCommand::Create(mesh, material, position, size, rotation);
            m_CommandBuffer.AddCommand(cmd);
        }
    }
    
    void Render()
    {
        m_PerformanceTracker.Reset();
        m_ShapeRenderer.BeginBatch();
        
        // 명령 실행
        m_CommandBuffer.Execute(m_ShapeRenderer);
        
        m_ShapeRenderer.EndBatch();
        m_PerformanceTracker.PrintReport();
        
        m_CommandBuffer.Clear();
    }
    
    // 기존 API 유지 (호환성)
    void AddRenderItem(const RenderItem& item) { /* 기존 구현 */ }
    SimpleMaterial* GetMaterial(const std::string& name) { /* 기존 구현 */ }
};
```

#### 6.2 사용 예시
```cpp
void ModernGameLoop()
{
    // 현대적인 API 사용
    renderer.Submit(circleMesh, "Red", {-0.5f, 0.0f});
    renderer.Submit(triangleMesh, "Blue", {0.5f, 0.0f});
    renderer.Submit(circleMesh, "Red", {0.0f, 0.5f});
    
    renderer.Render();
    
    // 출력 예상:
    // Material changed to: Red
    // Material changed to: Blue
    // State changes: 0 (최적화됨)
}
```

---

## 마이그레이션 가이드

### 기존 코드 호환성
```cpp
// 기존 방식 (여전히 동작)
RenderItem item;
item.mesh = ShapeFactory::CreateCircle();
item.position = {0.0f, 0.0f};
item.material = renderer.GetMaterial("Red");
renderer.AddRenderItem(item);

// 새로운 방식 (권장)
renderer.Submit(ShapeFactory::CreateCircle(), "Red", {0.0f, 0.0f});
```

### 단계별 적용
1. **1단계만 적용**: 다중 객체 렌더링 가능
2. **2단계까지**: 재질 기반 관리 가능
3. **3단계까지**: 명령 기반 렌더링 가능
4. **4단계까지**: 정렬 최적화 적용
5. **5단계까지**: 상태 관리 최적화 적용
6. **6단계**: 완전한 PSO 시스템 완성

### 성능 측정
각 단계별로 다음 메트릭을 측정:
- 렌더링 시간
- 상태 변경 횟수
- 메모리 사용량
- 드로우콜 수

---

## 결론

이 가이드를 통해 현재의 단순한 렌더링 시스템을 점진적으로 현대적인 PSO 시스템으로 발전시킬 수 있습니다. 각 단계는 독립적으로 동작하며, 필요에 따라 특정 단계에서 멈출 수도 있습니다.

가장 중요한 것은 **기존 코드의 호환성을 유지**하면서 **점진적으로 개선**하는 것입니다. 