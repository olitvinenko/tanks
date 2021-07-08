#include <exception>

//#include "../engine/ECS/ecsTest.h"

//static long xxx = _CrtSetBreakAlloc(12649);

//#include "audio/SoundEngine.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

glm::mat4 camera(float Translate, const glm::vec2& Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

#include <GameLoop.h>

#include <iostream>

struct FixedUpd : public IFixedUpdatable
{
    void FixedUpdate(float fixedDeltaTime) override
    {
        std::cout << "FixedUpdate " << fixedDeltaTime << std::endl;
    }
};

#include <thread>
#include <chrono>

#include "glfw/GlfwWindow.h"
#include "glfw/GlfwInput.h"
#include "glfw/GlfwClipboard.h"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <base/MouseButton.h>

struct TestUpdatable : public IUpdatable
{
    TestUpdatable(std::shared_ptr<IInput> input)
    : m_input(input)
    { }
    
    std::shared_ptr<IInput> m_input;
    
    void Update(float dt) override
    {
        if (m_input->GetMouseButton(MouseButton::Left))
        {
            std::cout << "Update m_input->GetMouseButton(MouseButton::Left)" << dt << std::endl;
        }
    }
};

struct TestRenderable : public IRenderable
{
    void Render(float fixedDeltaTime) override
    {
        std::cout << "Render " << fixedDeltaTime << std::endl;
        
        glClearColor(0, 0.4, 0, 1);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

struct InputConsoleListener : public IInputListener
{
    virtual void OnMouseButtonDown(MouseButton button) {
        std::cout << "OnMouseButtonDown" << std::endl;
    }
    virtual void OnMouseButtonUp(MouseButton button) {
        std::cout << "OnMouseButtonUp" << std::endl;
    }
    
//    virtual void OnMousePosition(float x, float y) {
//        std::cout << "OnMousePosition" << std::endl;
//    }
    
    virtual void OnMouseScrollOffset(float x, float y) {
        std::cout << "OnMouseScrollOffset" << std::endl;
    }
    
    virtual void OnCharacter(char character) {
        std::cout << "OnCharacter" << std::endl;
    }
    
    virtual void OnKeyDown(UI::Key key) {
        std::cout << "OnKeyDown" << std::endl;
    }
    virtual void OnKey(UI::Key key) {
        std::cout << "OnKey" << std::endl;
    }
    virtual void OnKeyUp(UI::Key key) {
        std::cout << "OnKeyUp" << std::endl;
    }
};

#include <Engine.h>

int main(int, const char**)
{
    auto window = std::make_shared<GlfwWindow>("test window", 1024, 768, false);
    auto input = std::make_shared<GlfwInput>(window.get());
    auto clipboard = std::make_shared<GlfwClipboard>(window.get());

    auto engine = std::make_unique<Engine>(input, clipboard, window);
    
    InputConsoleListener listener;
    engine->GetInput()->AddListener(&listener);
    
    auto& gameLoop = engine->GetGameLoop();
    
    {
        gameLoop->Add<FixedUpd>();
        gameLoop->Add<TestUpdatable>(input);
        gameLoop->Add<TestRenderable>();
        
        gameLoop->Start();
    }
    
    std::string clS;
    for (window->PollEvents(); !window->ShouldClose(); window->SwapBuffers())
    {
        input->Read();
        
        gameLoop->Tick();
        
        input->Clear();
    }

	return 0;
}
