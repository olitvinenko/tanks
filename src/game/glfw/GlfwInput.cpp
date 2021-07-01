#include "GlfwInput.h"
#include "GlfwWindow.h"

#include <base/MouseButton.h>
#include <base/Keys.h>
#include <math/Vect2D.h>

#include "GlfwKeys.h"

#include <GLFW/glfw3.h>

GlfwInput* GlfwInput::m_instance = nullptr;

bool GlfwInput::m_mouseButtons[static_cast<int>(MouseButton::Last)] = { false };
bool GlfwInput::m_keyboardButtons[static_cast<int>(UI::Key::Last)] = { false };

Vec2F GlfwInput::m_mousePosition;

GlfwInput::GlfwInput(GlfwWindow* window)
	: m_window(window)
{
	GLFWwindow* glfwWindow = m_window->m_glfwWindow;

	glfwSetMouseButtonCallback(glfwWindow, OnMouseButton);
	glfwSetCursorPosCallback(glfwWindow, OnCursorPos);
	glfwSetScrollCallback(glfwWindow, OnScroll);
	glfwSetKeyCallback(glfwWindow, OnKey);
	glfwSetCharCallback(glfwWindow, OnChar);

	m_instance = this;
}

void GlfwInput::AddListener(IInputListener* listener)
{
	m_listeners.insert(listener);
}

void GlfwInput::RemoveListener(IInputListener* listener)
{
	m_listeners.erase(listener);
}

void GlfwInput::Read()
{
	m_window->PollEvents();
}


GlfwInput::~GlfwInput()
{
	m_instance = nullptr;
}

void GlfwInput::Clear()
{
	memset(m_keyboardButtons, 0, static_cast<int>(UI::Key::Last) * sizeof(*m_keyboardButtons));
}

bool GlfwInput::GetKey(UI::Key key) const
{
	return m_keyboardButtons[static_cast<int>(key)];
}

bool GlfwInput::GetMouseButton(MouseButton button) const
{
	return m_mouseButtons[static_cast<int>(button)];
}

const Vec2F& GlfwInput::GetMousePosition() const
{
	return m_mousePosition;
}


Vec2F GetCursorPosInPixels(GLFWwindow *window, double dipX, double dipY)
{
    int pxWidth;
    int pxHeight;
    glfwGetFramebufferSize(window, &pxWidth, &pxHeight);

    int dipWidth;
    int dipHeight;
    glfwGetWindowSize(window, &dipWidth, &dipHeight);

    return{ float(dipX * pxWidth / dipWidth), float(dipY * pxHeight / dipHeight) };
}

Vec2F GetCursorPosInPixels(GLFWwindow *window)
{
    double dipX = 0;
    double dipY = 0;
    glfwGetCursorPos(window, &dipX, &dipY);
    return GetCursorPosInPixels(window, dipX, dipY);
}

// static members

void GlfwInput::OnMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	MouseButton unmapped;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		unmapped = MouseButton::Left;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		unmapped = MouseButton::Right;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		unmapped = MouseButton::Middle;
		break;
	default: return;
	}

	const int index = static_cast<int>(unmapped);

	m_mouseButtons[index] = action == GLFW_PRESS;

	switch (action)
	{
	case GLFW_RELEASE:
		for (auto listener : m_instance->m_listeners)
			listener->OnMouseButtonUp(unmapped);
		break;

	case GLFW_PRESS:
		for (auto listener : m_instance->m_listeners)
			listener->OnMouseButtonDown(unmapped);
		break;
	}
}

void GlfwInput::OnKey(GLFWwindow *window, int platformKey, int scancode, int action, int mods)
{
	UI::Key key = MapGlfwKeyCode(platformKey);
	const int index = static_cast<int>(key);

	m_keyboardButtons[index] = action == GLFW_REPEAT;

	switch (action)
	{
	case GLFW_REPEAT:
		for (auto listener : m_instance->m_listeners)
			listener->OnKey(key);
		break;

	case GLFW_RELEASE:
		for (auto listener : m_instance->m_listeners)
			listener->OnKeyUp(key);
		break;

	case GLFW_PRESS:
		for (auto listener : m_instance->m_listeners)
			listener->OnKeyDown(key);
		break;
	}
}

void GlfwInput::OnScroll(GLFWwindow *window, double xOffset, double yOffset)
{
	int pxWidth;
	int pxHeight;
	glfwGetFramebufferSize(window, &pxWidth, &pxHeight);

	int dipWidth;
	int dipHeight;
	glfwGetWindowSize(window, &dipWidth, &dipHeight);

	for (auto listener : m_instance->m_listeners)
		listener->OnMouseScrollOffset(float(xOffset * pxWidth / dipWidth), float(yOffset * pxHeight / dipHeight));
}

void GlfwInput::OnCursorPos(GLFWwindow *window, double xpos, double ypos)
{
    Vec2F mousePos = GetCursorPosInPixels(window, xpos, ypos);
    
    m_mousePosition = mousePos;

	for (auto listener : m_instance->m_listeners)
		listener->OnMousePosition(mousePos.x, mousePos.y);
}

void GlfwInput::OnChar(GLFWwindow *window, unsigned int codepoint)
{
    const char character = static_cast<char>(codepoint);

    if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9'))
    {
        for (auto listener : m_instance->m_listeners)
            listener->OnCharacter(character);
        return;
    }

    switch (character)
    {
    case ' ':
    case '_':
    case '-':
        for (auto listener : m_instance->m_listeners)
            listener->OnCharacter(character);
        break;

    default: break;
    }

    if (codepoint < 57344 || codepoint > 63743) // ignore Private Use Area characters
        for (auto listener : m_instance->m_listeners)
            listener->OnCharacter(character);
}
