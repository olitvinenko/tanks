#pragma once

#include <stack>
#include <string>

#include "math/Rect.h"
#include "base/IRender.h"

struct Color;
class TextureManager;


enum AlignTextKind
{
	  alignTextLT = 0
	, alignTextCT = 1
	, alignTextRT = 2
	, alignTextLC = 3
	, alignTextCC = 4
	, alignTextRC = 5
	, alignTextLB = 6
	, alignTextCB = 7
	, alignTextRB = 8
};

class DrawingContext
{
public:
    DrawingContext(const TextureManager &tm, IRender* render, unsigned int width, unsigned int height);

    void PushClippingRect(RectInt rect);
    void PopClippingRect();

    void PushTransform(Vec2F offset, float opacityCombined = 1);
    void PopTransform();

    RectInt GetVisibleRegion() const;

    void DrawSprite(const RectFloat dst, size_t sprite, Color color, unsigned int frame);
    void DrawBorder(const RectFloat &dst, size_t sprite, Color color, unsigned int frame);
    void DrawBitmapText(Vec2F origin, float scale, size_t tex, Color color, const std::string &str, AlignTextKind align = alignTextLT);
    void DrawSprite(size_t tex, unsigned int frame, Color color, float x, float y, Vec2F dir);
    void DrawSprite(size_t tex, unsigned int frame, Color color, float x, float y, float width, float height, Vec2F dir);
    void DrawIndicator(size_t tex, float x, float y, float value);
    void DrawLine(size_t tex, Color color, float x0, float y0, float x1, float y1, float phase);
    void DrawBackground(size_t tex, RectFloat bounds) const;

    void DrawPointLight(float intensity, float radius, Vec2F pos);
    void DrawSpotLight(float intensity, float radius, Vec2F pos, Vec2F dir, float offset, float aspect);
    void DrawDirectLight(float intensity, float radius, Vec2F pos, Vec2F dir, float length);

    void Camera(RectInt viewport, float x, float y, float scale);
    void SetAmbient(float ambient);
    void SetMode(const RenderMode mode);

private:
    struct Transform
    {
        Vec2F offset;
        uint32_t opacity;
    };
    const TextureManager &_tm;
    IRender* _render;
    std::stack<RectInt> _clipStack;
    std::stack<Transform> _transformStack;
    RectInt _viewport;
    RenderMode _mode;
};
