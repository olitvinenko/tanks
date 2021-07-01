#pragma once

#include <string>

using Vec2F = Vec2D<float32>;

struct IWindowListener
{
	virtual ~IWindowListener() = default;

	virtual void OnFrameBufferChanged(int width, int height) { }
	virtual void OnSizeChanged(int width, int height) { }
	virtual void OnClosed() { }
};

struct IWindow
{
	virtual ~IWindow() = default;

	virtual void AddListener(IWindowListener* listener) = 0;
	virtual void RemoveListener(IWindowListener* listener) = 0;

	virtual int GetPixelWidth() const = 0;
	virtual int GetPixelHeight() const = 0;
    virtual float GetLayoutScale() const = 0;
	virtual float GetAspectRatio() const = 0;

	virtual bool ShouldClose() const = 0;
	virtual void SwapBuffers() = 0;
	virtual void PollEvents() = 0;

	virtual const std::string& GetName() const = 0;
    
    virtual Vec2F GetCursorPosInPixels() const = 0;
    virtual Vec2F GetCursorPosInPixels(double xoffset, double yoffset) const = 0;
    virtual Vec2F GetPixelSize() const = 0;
};
