#pragma once

class Renderer {
public:
    void Init();
    void BeginFrame();
    void EndFrame();
    void Close();
    bool ShouldClose() const;
};
