//
// Created by Dhava on 18-11-2023.
//

#ifndef DEEX_TEXTDATA_H
#define DEEX_TEXTDATA_H

#include "Cursor.h"
#include "Font.h"
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "helpers.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "texture.h"
#include <memory>
#include <string>
#include <vector>

struct TextVertex
{
    glm::vec2 pos;
    glm::vec2 texCoords;
    uint32_t character;
};

struct TextData
{
    std::string textBuffer;             //\n terminated
    std::vector<TextVertex> vertexData; // cpu side vertex data , maintain consistency with gpu side
    Font font;
    uint32_t vboId{};
    uint32_t vaoId{};
    double fontSize;
    double fontScale;
    double lineHeight;

    TextData();

    void setText(std::string str)
    {
        textBuffer = std::move(str);
    }

    void appendText(const std::string &str)
    {
        textBuffer.append(str);
    }
    void appendText(const char *str)
    {
        textBuffer.append(str);
    }
    void appendChar(char c)
    {
        textBuffer.push_back(c);
    }

    void insertText(int index, const std::string &str)
    {
        textBuffer.insert(index, str);
    }
    void insertText(int index, const char *str)
    {
        textBuffer.insert(index, str);
    }
    void insertChar(int index, char c)
    {
        textBuffer.insert(index, 1, c);
    }

    // dont allow to delete last \n
    void deleteText(int index, int count)
    {
        textBuffer.erase(index, count);
        if (textBuffer.back() != '\n')
            textBuffer.push_back('\n');
    }
    void deleteChar(int index)
    {
        if (index >= 0 and index < textBuffer.size() - 1)
            textBuffer.erase(index, 1);
    }

    glm::vec2 getXYCoordFromBufferIndex(int textIndex) const;
    void render();

    void updateRenderDataStartingFrom(uint32_t leftMostUpdated);
};

#endif // DEEX_TEXTDATA_H
