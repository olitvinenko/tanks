#pragma once

#include <base/IClipboard.h>

class GlfwWindow;

class GlfwClipboard final : public IClipboard
{
public:
    explicit GlfwClipboard(GlfwWindow* window);
    
    std::string GetText() override;
    void SetText(const std::string& text) override;
    
private:
    GlfwWindow* m_window;
};
