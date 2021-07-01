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
    int i = 0;
    
    FixedUpd(int ii) : i(ii)
    {
    }
    
    ~FixedUpd()
    {
    }
    
    void FixedUpdate(float fixedDeltaTime) override
    {
        std::cout << "FixedUpdate " << fixedDeltaTime << std::endl;
    }
};

struct Upd : public IUpdatable
{
    void Update(float fixedDeltaTime) override
    {
        std::cout << "Update " << fixedDeltaTime << std::endl;
    }
};

struct Rend : public IRenderable
{
    void Render(float fixedDeltaTime) override
    {
        std::cout << "Render " << fixedDeltaTime << std::endl;
    }
};

#include <thread>
#include <chrono>

//#include <base/IWindow.h>
#include "glfw/GlfwWindow.h"
#include "glfw/GlfwInput.h"
#include "glfw/GlfwClipboard.h"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct InputListener : public IInputListener
{
    virtual void OnMouseButtonDown(MouseButton button) {
        std::cout << "OnMouseButtonDown" << std::endl;
    }
    virtual void OnMouseButtonUp(MouseButton button) {
        std::cout << "OnMouseButtonUp" << std::endl;
    }
    
    virtual void OnMousePosition(float x, float y) {
        std::cout << "OnMousePosition" << std::endl;
    }
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

#include <base/Keys.h>

#include <string>
int main(int, const char**)
{
    auto window = std::make_shared<GlfwWindow>("test window", 940, 640, false);
    
    InputListener listener;
    IInput* input = new GlfwInput(window.get());
    input->AddListener(&listener);
    
    IClipboard* cl = new GlfwClipboard(window.get());
    std::string clS;
    
    for (window->PollEvents(); !window->ShouldClose();)
    {
        input->Read();
        
        glClearColor(0, 0.4, 0, 1);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (window->IsFocused())
        {
            clS = cl->GetText();
            if (!clS.empty())
            {
                std::cout << "clip " << clS << std::endl;
                clS.clear();
                cl->SetText("");
            }
            
            if (input->GetKey(UI::Key::A))
            {
                std::cout << "aaaaaa " << clS << std::endl;
            }
        }
        
        window->SwapBuffers();
        input->Clear();
    }
    
    
    return 0;
    
    
    static_assert(std::is_base_of<IFixedUpdatable, FixedUpd>::value, "");
    
    GameLoop gl;
    
    auto fixedUpd = gl.Add<FixedUpd>(10);
    gl.Add<Rend>();
    gl.Add<Upd>();
    
    gl.Start();
    for (int i = 0; i < 1000000; ++i)
    {
        gl.Tick();
        
//        for (int j = 0; j < 1000000; ++j)
//        { }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    
    std::weak_ptr<FixedUpd> upd2 = fixedUpd;
    gl.Remove(fixedUpd.lock().get());
    gl.Remove(fixedUpd);

	return 0;
}
