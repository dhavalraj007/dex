//
// Created by Dhava on 13-11-2023.
//

#include "texture.h"
#include <filesystem>

#include "glad/glad.h"
#include "stb_image.h"
#include "stb_image_write.h"

Texture::Texture() : m_Id(0), m_TexUnit(1)
{
}

Texture::Texture(const std::filesystem::path &path, int texUnit) : m_Id(0), m_TexUnit(texUnit)
{
    // MCLONE_ASSERT(m_TexUnit > 0, "Texture Unit 0 is not available for User acess. - please use texture units >= 1");
    int width, height, numOfChannels;
    stbi_set_flip_vertically_on_load(1);
    std::string cpath = path.string();
    const unsigned char *data = stbi_load(cpath.c_str(), &width, &height, &numOfChannels, 0);

    if (data)
    {
        specs.Width = width;
        specs.Height = height;
        if (numOfChannels == 1)
            specs.ChannelFormat = GL_RED;
        else if (numOfChannels == 2)
            specs.ChannelFormat = GL_RG;
        else if (numOfChannels == 3)
            specs.ChannelFormat = GL_RGB;
        else if (numOfChannels == 4)
            specs.ChannelFormat = GL_RGBA;
        else
            specs.ChannelFormat = GL_RGB;
    }
    else
    {
        // MCLONE_ERROR("Error loading texture from path :{}", path);
    }
    LoadTexture(data);
}

Texture::Texture(Texture::TextureSpecs specs, int texUnit) : specs(specs), m_Id(0), m_TexUnit(texUnit)
{
}

Texture::~Texture()
{
}

void Texture::setMagFilter(TextureFiltering filter)
{
    specs.MagFilter = filter;
    glActiveTexture(GL_TEXTURE0 + m_TexUnit);
    glBindTexture(GL_TEXTURE_2D, m_Id); // MCLONE_CHECK_GL_ERROR;
    switch (specs.MagFilter)
    {
    case TextureFiltering::Nearest:
        break;
    case TextureFiltering::Linear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MCLONE_CHECK_GL_ERROR
        break;
    default:
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0); // MCLONE_CHECK_GL_ERROR;
}

void Texture::setMinFilter(TextureFiltering filter)
{
    specs.MinFilter = filter;
    glActiveTexture(GL_TEXTURE0 + m_TexUnit);
    glBindTexture(GL_TEXTURE_2D, m_Id); // MCLONE_CHECK_GL_ERROR;
    switch (specs.MinFilter)
    {
    case TextureFiltering::Nearest:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // MCLONE_CHECK_GL_ERROR
        break;
    case TextureFiltering::Linear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // MCLONE_CHECK_GL_ERROR
        break;
    default:
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0); // MCLONE_CHECK_GL_ERROR;
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0 + m_TexUnit);
    glBindTexture(GL_TEXTURE_2D, m_Id); // MCLONE_CHECK_GL_ERROR;
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadTexture(const unsigned char *data)
{
    if (!data)
    {
        data = new unsigned char[4 * 4 * 3]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
        specs.Width = 4;
        specs.Height = 4;
        specs.ChannelFormat = GL_RGB;
    }

    glGenTextures(1, &m_Id); // MCLONE_CHECK_GL_ERROR;
    glActiveTexture(GL_TEXTURE0 + m_TexUnit);
    glBindTexture(GL_TEXTURE_2D, m_Id); // MCLONE_CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);

    glTexImage2D(GL_TEXTURE_2D, 0, specs.ChannelFormat, specs.Width, specs.Height, 0, specs.ChannelFormat,
                 GL_UNSIGNED_BYTE,
                 data); // MCLONE_CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
    glGenerateMipmap(GL_TEXTURE_2D);
    setMagFilter(specs.MagFilter);
    setMinFilter(specs.MinFilter);
    // MCLONE_TRACE("Loaded {}-Channel Texture :{}", specs.NumOfChannels, specs.Path);
    glBindTexture(GL_TEXTURE_2D, 0);
}
