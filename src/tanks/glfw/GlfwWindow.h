#pragma once

#include <base/IWindow.h>
#include <set>

struct GLFWwindow;

class GlfwWindow final : public IWindow
{
public:
	GlfwWindow(const char* title, int width, int height, bool fullscreen);
	~GlfwWindow() override;

	int GetPixelWidth() const override;

	void AddListener(IWindowListener* listener) override;
	void RemoveListener(IWindowListener* listener) override;

	int GetPixelHeight() const override;
	float GetAspectRatio() const override;
    float GetLayoutScale() const override;

	bool ShouldClose() const override;
	void SwapBuffers() override;

	void PollEvents() override;
    
    bool IsFocused() const override { return m_isFocused; }
    const std::string& GetName() const override { return m_name; }

    Vec2F GetCursorPosInPixels() const override;
    Vec2F GetCursorPosInPixels(double xoffset, double yoffset) const override;
    Vec2F GetPixelSize() const override;

private:

	static void OnFramebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void OnSizeCallback(GLFWwindow *window, int width, int height);
	static void OnCloseCallback(GLFWwindow *window);
    static void OnFocusedCallback(GLFWwindow* window, int focused);

	GLFWwindow* m_glfwWindow;
	std::string m_name;
	std::set<IWindowListener*> m_listeners;
    bool m_isFocused;

	friend class GlfwClipboard;
	friend class GlfwInput;

	static GlfwWindow* m_instance;
};
