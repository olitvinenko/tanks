#pragma once

#include <list>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "base/IRender.h"
#include "GlTexture.h"

namespace FileSystem
{
    class IFileSystem;
    class Memory;
}

class TextureManager
{
public:
    struct LogicalTexture
    {
        Vec2F uvPivot;

        float pxFrameWidth;
        float pxFrameHeight;
        float pxBorderSize;
        
        bool magFilter;

        std::vector<RectFloat> uvFrames;
    };
    
    TextureManager(TextureManager&&) = default;
    explicit TextureManager(IRender& render);
    ~TextureManager();

    int LoadPackage(std::vector<std::tuple<std::shared_ptr<IImage>, std::string, LogicalTexture>> definitions);
    void UnloadAllTextures();

    size_t FindSprite(const std::string &name) const;
    const GlTexture& GetDeviceTexture(size_t texIndex) const { return _logicalTextures[texIndex].second->id; }
    const LogicalTexture& GetSpriteInfo(size_t texIndex) const { return _logicalTextures[texIndex].first; }
    float GetFrameWidth(size_t texIndex, size_t /*frameIdx*/) const { return _logicalTextures[texIndex].first.pxFrameWidth; }
    float GetFrameHeight(size_t texIndex, size_t /*frameIdx*/) const { return _logicalTextures[texIndex].first.pxFrameHeight; }
    float GetBorderSize(size_t texIndex) const { return _logicalTextures[texIndex].first.pxBorderSize; }
    unsigned int GetFrameCount(size_t texIndex) const { return static_cast<unsigned int>(_logicalTextures[texIndex].first.uvFrames.size()); }

    void GetTextureNames(std::vector<std::string> &names, const char *prefix) const;

    float GetCharHeight(size_t fontTexture) const;

private:
    IRender& _render;

    struct TexDesc
    {
        GlTexture id;
        int width;          // The Width Of The Entire Image.
        int height;         // The Height Of The Entire Image.
        int refCount;       // number of logical textures
    };

    std::list<TexDesc> _devTextures;
    std::map<std::shared_ptr<IImage>, std::list<TexDesc>::iterator> _mapImage_to_TexDescIter;
    std::map<std::string, size_t> _mapName_to_Index;// index in _logicalTextures
    std::vector<std::pair<LogicalTexture, std::list<TexDesc>::iterator>> _logicalTextures;

    std::list<TexDesc>::iterator LoadTexture(const std::shared_ptr<IImage> &image, bool magFilter);

    void CreateChecker(); // Create checker texture without name and with index=0
};

std::vector<std::tuple<std::shared_ptr<IImage>, std::string, TextureManager::LogicalTexture>>
ParsePackage(const std::string &packageName, std::shared_ptr<FileSystem::Memory> file, FileSystem::IFileSystem &fs);

std::vector<std::tuple<std::shared_ptr<IImage>, std::string, TextureManager::LogicalTexture>>
ParseDirectory(const std::string &dirName, const std::string &texPrefix, FileSystem::IFileSystem &fs);
