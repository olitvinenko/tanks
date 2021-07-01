#pragma once

#include <set>

#include <base/IInput.h>

class GlfwWindow;
struct GLFWwindow;
namespace UI
{
    enum class Key;
}
enum class MouseButton;

class GlfwInput final : public IInput
{
public:
    explicit GlfwInput(GlfwWindow* window);
    ~GlfwInput();
    
    void Read() override;
    void Clear() override;
    
    void AddListener(IInputListener *listener) override;
    void RemoveListener(IInputListener *listener) override;
    
    bool GetKey(UI::Key key) const override;
    bool GetMouseButton(MouseButton button) const override;
    const Vec2F& GetMousePosition() const override;

private:
    static void OnMouseButton(GLFWwindow *window, int button, int action, int mods);
    static void OnKey(GLFWwindow *window, int platformKey, int scancode, int action, int mods);
    static void OnScroll(GLFWwindow *window, double xOffset, double yOffset);
    static void OnCursorPos(GLFWwindow *window, double xpos, double ypos);
    static void OnChar(GLFWwindow *window, unsigned int codepoint);
    
    GlfwWindow* m_window;
    std::set<IInputListener*> m_listeners;
    
    static Vec2F m_mousePosition;
    static bool m_mouseButtons[];
    static bool m_keyboardButtons[];
    
    static GlfwInput* m_instance;
};
