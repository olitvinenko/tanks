#include "GlfwClipboard.h"
#include "GlfwWindow.h"
#include "GLFW/glfw3.h"

GlfwClipboard::GlfwClipboard(GlfwWindow* window)
    : m_window(window)
{ }

std::string GlfwClipboard::GetText()
{
    return glfwGetClipboardString(m_window->m_glfwWindow);
}

void GlfwClipboard::SetText(const std::string& text)
{
    glfwSetClipboardString(m_window->m_glfwWindow, text.c_str());
}
