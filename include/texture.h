//
// Created by Dhava on 13-11-2023.
//

#ifndef DEEX_TEXTURE_H
#define DEEX_TEXTURE_H

#pragma once

#include "glad/glad.h"
#include <cstdint>
#include <filesystem>
#include <string>

class Texture
{
  public:
    enum class TextureFiltering
    {
        Nearest,
        Linear
    };

    struct TextureSpecs
    {
        uint32_t Width = 4;
        uint32_t Height = 4;
        GLenum ChannelFormat = GL_RGB;

        TextureFiltering MagFilter = TextureFiltering::Linear;
        TextureFiltering MinFilter = TextureFiltering::Nearest;
    };

    Texture();
    Texture(const std::filesystem::path &path, int texUnit);
    Texture(TextureSpecs specs, int texUnit);
    ~Texture();

    [[nodiscard]] inline int getTexUnit() const
    {
        return m_TexUnit;
    }
    void bind() const;
    void unbind();

    void LoadTexture(const unsigned char *data);
    TextureSpecs specs;

  private:
    uint32_t m_Id;
    int m_TexUnit;

    void setMagFilter(TextureFiltering filter);
    void setMinFilter(TextureFiltering filter);
};

#endif // DEEX_TEXTURE_H