#pragma once
struct Input;

struct IStageLogic {
    virtual ~IStageLogic() = default;
    virtual void Initialize() = 0;
    virtual void Update(Input* input) = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
};
