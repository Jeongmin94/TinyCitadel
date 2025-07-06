# OpenGL 기반 PSO 설계 전략

## 개요

현재 TinyCitadel 프로젝트는 OpenGL을 사용하고 있지만, Vulkan의 Pipeline State Object(PSO) 개념을 차용하여 더 효율적이고 확장 가능한 렌더링 시스템을 구축합니다.

## 목표

1. **상태 관리 최적화**: 렌더링 상태 변경 최소화
2. **배치 렌더링**: 같은 상태를 사용하는 객체들을 묶어서 처리
3. **확장성**: 새로운 재질과 렌더링 기법 쉽게 추가
4. **성능**: CPU 오버헤드 감소 및 GPU 활용도 향상

## 단계별 구현 계획

### 1단계: 렌더링 상태 추상화

#### 1.1 RenderState 구조체 정의
```cpp
struct RenderState
{
    GLuint shaderProgram = 0;
    GLenum blendSrc = GL_ONE;
    GLenum blendDst = GL_ZERO;
    bool enableBlend = false;
    bool enableDepthTest = true;
    GLenum depthFunc = GL_LESS;
    bool enableCulling = true;
    GLenum cullFace = GL_BACK;
    
    // 해시 값 계산용
    size_t GetHash() const;
    bool operator==(const RenderState& other) const;
};
```

#### 1.2 RenderStateManager 클래스 생성
```cpp
class RenderStateManager
{
private:
    RenderState m_CurrentState;
    std::unordered_map<size_t, RenderState> m_StateCache;
    
public:
    void SetRenderState(const RenderState& state);
    void CreatePredefinedStates();
    const RenderState& GetState(const std::string& name) const;
};
```

### 2단계: 재질 시스템 구축

#### 2.1 Material 클래스 정의
```cpp
class Material
{
private:
    RenderState m_RenderState;
    std::unordered_map<std::string, glm::vec4> m_Vec4Properties;
    std::unordered_map<std::string, float> m_FloatProperties;
    std::unordered_map<std::string, GLuint> m_TextureProperties;
    
public:
    void SetProperty(const std::string& name, const glm::vec4& value);
    void SetProperty(const std::string& name, float value);
    void SetTexture(const std::string& name, GLuint texture);
    
    void Bind(RenderStateManager& stateManager) const;
    const RenderState& GetRenderState() const { return m_RenderState; }
};
```

#### 2.2 기본 재질 정의
- **SolidMaterial**: 단색 렌더링
- **AlphaMaterial**: 반투명 렌더링
- **WireframeMaterial**: 와이어프레임 렌더링
- **TexturedMaterial**: 텍스처 렌더링

### 3단계: 렌더링 커맨드 시스템

#### 3.1 RenderCommand 구조체
```cpp
struct RenderCommand
{
    Mesh mesh;
    Material* material;
    glm::mat4 modelMatrix;
    int layer = 0;
    
    // 정렬을 위한 키 생성
    uint64_t GetSortKey() const;
};
```

#### 3.2 RenderQueue 클래스
```cpp
class RenderQueue
{
private:
    std::vector<RenderCommand> m_Commands;
    
public:
    void AddCommand(const RenderCommand& command);
    void Sort();  // 상태별, 레이어별 정렬
    void Execute(RenderStateManager& stateManager, const glm::mat4& viewProjMatrix);
    void Clear();
};
```

### 4단계: 배치 렌더링 시스템

#### 4.1 RenderBatch 구조체
```cpp
struct RenderBatch
{
    Material* material;
    std::vector<RenderCommand*> commands;
    
    void Execute(RenderStateManager& stateManager, const glm::mat4& viewProjMatrix);
};
```

#### 4.2 BatchRenderer 클래스
```cpp
class BatchRenderer
{
private:
    std::unordered_map<Material*, RenderBatch> m_Batches;
    
public:
    void AddCommand(const RenderCommand& command);
    void ExecuteBatches(RenderStateManager& stateManager, const glm::mat4& viewProjMatrix);
    void Clear();
};
```

### 5단계: 통합 렌더링 시스템

#### 5.1 확장된 Renderer 클래스
```cpp
class Renderer
{
private:
    RenderStateManager m_StateManager;
    RenderQueue m_OpaqueQueue;
    RenderQueue m_TransparentQueue;
    BatchRenderer m_BatchRenderer;
    
    // 카메라 시스템
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    
public:
    // 렌더링 제출
    void Submit(const Mesh& mesh, Material* material, const glm::mat4& transform, int layer = 0);
    void Submit(const RenderCommand& command);
    
    // 카메라 설정
    void SetCamera(const glm::mat4& view, const glm::mat4& projection);
    
    // 렌더링 실행
    void Render();
    
    // 프레임 시작/종료
    void BeginFrame();
    void EndFrame();
};
```

## 구현 순서

### Phase 1: 기본 인프라 (1-2주)
1. `RenderState` 구조체 구현
2. `RenderStateManager` 클래스 구현
3. 기본 상태 전환 테스트

### Phase 2: 재질 시스템 (2-3주)
1. `Material` 클래스 구현
2. 기본 재질들 구현
3. 쉐이더 프로퍼티 바인딩 시스템

### Phase 3: 커맨드 시스템 (2-3주)
1. `RenderCommand` 구조체 구현
2. `RenderQueue` 클래스 구현
3. 정렬 알고리즘 최적화

### Phase 4: 배치 렌더링 (3-4주)
1. `RenderBatch` 구조체 구현
2. `BatchRenderer` 클래스 구현
3. 인스턴싱 지원 (선택사항)

### Phase 5: 통합 및 최적화 (2-3주)
1. 기존 `Renderer` 클래스 리팩토링
2. 성능 테스트 및 최적화
3. 디버깅 도구 추가

## 예상 아키텍처

```
┌─────────────────────────────────────────────────────────────┐
│                        Renderer                             │
├─────────────────────────────────────────────────────────────┤
│ - RenderStateManager                                        │
│ - RenderQueue (Opaque, Transparent)                        │
│ - BatchRenderer                                             │
│ - Camera System                                             │
└─────────────────────────────────────────────────────────────┘
                               │
                               ▼
┌─────────────────────┐    ┌─────────────────────┐    ┌─────────────────────┐
│  RenderStateManager │    │    RenderQueue      │    │   BatchRenderer     │
├─────────────────────┤    ├─────────────────────┤    ├─────────────────────┤
│ - RenderState[]     │    │ - RenderCommand[]   │    │ - RenderBatch[]     │
│ - StateCache        │    │ - Sort()            │    │ - Group by Material │
│ - SetState()        │    │ - Execute()         │    │ - Execute()         │
└─────────────────────┘    └─────────────────────┘    └─────────────────────┘
                               │
                               ▼
┌─────────────────────┐    ┌─────────────────────┐
│    RenderCommand    │    │      Material       │
├─────────────────────┤    ├─────────────────────┤
│ - Mesh              │    │ - RenderState       │
│ - Material*         │    │ - Properties        │
│ - ModelMatrix       │    │ - Textures          │
│ - Layer             │    │ - Bind()            │
└─────────────────────┘    └─────────────────────┘
```

## 성능 최적화 전략

### 1. 상태 변경 최소화
- 현재 상태를 캐시하고 불필요한 OpenGL 호출 제거
- 상태 해시를 이용한 빠른 비교

### 2. 드로우콜 배치화
- 같은 재질을 사용하는 객체들을 그룹화
- 인스턴싱을 통한 다중 객체 한 번에 렌더링

### 3. 메모리 최적화
- 객체 풀링을 통한 메모리 할당 최소화
- SOA(Structure of Arrays) 데이터 레이아웃 고려

### 4. CPU-GPU 병렬화
- 프레임 n+1의 준비 작업을 프레임 n 렌더링과 병렬 처리
- 비동기 리소스 로딩

## 호환성 고려사항

### OpenGL 4.1 제한사항
- 컴퓨트 쉐이더 미지원
- 일부 최신 기능 제한
- macOS 호환성 우선

### 기존 코드와의 호환성
- 현재 `ShapeFactory`, `ShapeRenderer` 구조 유지
- 점진적 마이그레이션 가능
- 기존 API와 새 API 병행 지원

## 디버깅 및 프로파일링

### 렌더링 디버그 정보
- 상태 변경 횟수 추적
- 드로우콜 수 모니터링
- 배치 효율성 측정

### 시각화 도구
- 렌더링 큐 시각화
- 재질별 객체 분포 표시
- 성능 메트릭 실시간 표시

## 결론

이 설계 전략을 통해 OpenGL 기반 프로젝트에서도 Vulkan의 PSO 시스템의 장점을 활용할 수 있습니다. 단계적 구현을 통해 기존 코드를 점진적으로 개선하면서 성능과 확장성을 모두 확보할 수 있습니다.

## 참고 자료

- [OpenGL State Management Best Practices](https://www.khronos.org/opengl/wiki/OpenGL_State_Management)
- [Vulkan Pipeline State Objects](https://vulkan.lunarg.com/doc/view/1.0.30.0/linux/vkspec.html#pipelines)
- [Modern OpenGL Rendering Techniques](https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL) 