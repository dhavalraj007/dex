#include "TextData.h"
#include <cctype>

#include "Font.h"

void TextData::render()
{
    glBindVertexArray(vaoId);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
}

TextData::TextData() : fontSize(30), font("../MouldyCheeseRegular-WyMWG.ttf")
{
    const double maxSize = (font.fontGeometry.getMetrics().ascenderY - font.fontGeometry.getMetrics().descenderY);
    fontScale = 1.0 / maxSize;
    lineHeight = fontScale * font.fontGeometry.getMetrics().lineHeight;

    const int defaultNumVerts = 300;
    vertexData.reserve(defaultNumVerts);
    glGenBuffers(1, &vboId);
    CHECK_GL_ERROR;
    glGenVertexArrays(1, &vaoId);
    CHECK_GL_ERROR;

    glBindVertexArray(vaoId);
    CHECK_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    CHECK_GL_ERROR;
    glBufferData(GL_ARRAY_BUFFER, defaultNumVerts * sizeof(TextVertex), nullptr, GL_DYNAMIC_DRAW);
    CHECK_GL_ERROR;

    // pos attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void *)0);
    CHECK_GL_ERROR;
    glEnableVertexAttribArray(0);
    CHECK_GL_ERROR;
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void *)(sizeof(TextVertex::pos)));
    CHECK_GL_ERROR;
    glEnableVertexAttribArray(1);
    CHECK_GL_ERROR;
    // character
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(TextVertex),
                           (void *)(sizeof(TextVertex::pos) + sizeof(TextVertex::texCoords)));
    CHECK_GL_ERROR;
    glEnableVertexAttribArray(2);
    CHECK_GL_ERROR;
}

glm::vec2 TextData::getXYCoordFromBufferIndex(int textIndex) const
{
    auto geo = font.fontGeometry.getGlyph(font.hasGlyph(textBuffer[textIndex])
                                              ? textBuffer[textIndex]
                                              : ' '); // as a default load space (for \n also space will be used)
    double temp_pl, temp_pb, temp_pr, temp_pt;
    geo->getQuadPlaneBounds(temp_pl, temp_pb, temp_pr, temp_pt);
    int vertexIndex = textIndex * 6;
    float x = vertexData[vertexIndex].pos.x - (temp_pl * fontScale);
    float y = vertexData[vertexIndex].pos.y - (temp_pb * fontScale);
    return {x, y};
}

void TextData::updateRenderDataStartingFrom(uint32_t start)
{
    if (start >= textBuffer.size())
        return;
    if (start < 0)
        start = 0;
    LOG("updating renderData starting from {} which is {}", start, textBuffer[start]);
    // for local space scaling
    const double maxSize = (font.fontGeometry.getMetrics().ascenderY - font.fontGeometry.getMetrics().descenderY);
    const double fontScale = 1.0 / maxSize;

    int vertexIndex = start * 6;
    vertexData.erase(vertexIndex + vertexData.begin(), vertexData.end());

    // where to start coords from
    //  default 0,0
    double x = 0;
    double y = 0;

    if (start >= 1)
    { // if we have atleast one char to the left
        auto tempXY = getXYCoordFromBufferIndex(start - 1);
        // start coords from the previous char's pos
        x = tempXY.x;
        y = tempXY.y;
        if (textBuffer[start - 1] == '\n')
        { // if the prev char is \n then offset it to up by lineheight and to the left at 0
            x = 0;
            y -= lineHeight;
        }
    }

    // set advance for start and start-1 character
    double advance = 0;
    if (start >= 1)
    {
        auto curr = textBuffer[start];
        auto prev = textBuffer[start - 1];
        if (!std::isprint(curr))
            curr = ' '; // treat non-printables chars as ' '
        font.fontGeometry.getAdvance(advance, prev, curr);
        if (prev == '\n')
            advance = 0; // if prev is \n means this is newline thus dont add advance
    }
    x += fontScale * advance;

    // from start to the end
    for (int i = start; i < textBuffer.size(); i++)
    {
        auto character = textBuffer[i];

        // get geometry of character, as a default load space (for \n also space will be used)
        const msdf_atlas::GlyphGeometry *geometry =
            font.fontGeometry.getGlyph(font.hasGlyph(character) ? character : ' ');
        // as a default load space (for \n also space will be used)
        double al, ab, ar, at;
        geometry->getQuadAtlasBounds(al, ab, ar, at);
        // scale to 0..1
        al *= (1.0 / font.texture->specs.Width);
        ab *= (1.0 / font.texture->specs.Height);
        ar *= (1.0 / font.texture->specs.Width);
        at *= (1.0 / font.texture->specs.Height);

        double pl, pb, pr, pt;
        geometry->getQuadPlaneBounds(pl, pb, pr, pt);
        pl *= fontScale;
        pb *= fontScale;
        pr *= fontScale;
        pt *= fontScale;
        pl += x;
        pr += x;
        pb += y;
        pt += y;

        TextVertex vertexLB{glm::vec2(pl, pb), glm::vec2(al, ab), (uint32_t)character};
        TextVertex vertexRB{glm::vec2(pr, pb), glm::vec2(ar, ab), (uint32_t)character};
        TextVertex vertexLT{glm::vec2(pl, pt), glm::vec2(al, at), (uint32_t)character};
        TextVertex vertexRT{glm::vec2(pr, pt), glm::vec2(ar, at), (uint32_t)character};
        vertexData.push_back(vertexLB);
        vertexData.push_back(vertexRB);
        vertexData.push_back(vertexLT);
        vertexData.push_back(vertexLT);
        vertexData.push_back(vertexRB);
        vertexData.push_back(vertexRT);

        if (character == '\r' or character == '\n')
        {
            x = 0;
            y -= lineHeight;
            continue;
        }

        // add advance
        double advance = geometry->getAdvance();
        char nextCharacter = textBuffer[i + 1];
        if (!font.hasGlyph(nextCharacter))
            nextCharacter = ' '; // if we dont have glyph for this (including \n) treat it as ' '
        font.fontGeometry.getAdvance(advance, character, nextCharacter);
        x += advance * fontScale;
    }

    // re-allocate memory according to vertexData's capacity
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    GLint buffer_size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
    if (buffer_size >= sizeof(TextVertex) * vertexData.capacity())
    {
        glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(TextVertex),
                        (vertexData.size() - vertexIndex) * sizeof(TextVertex), &vertexData[vertexIndex]);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, vertexData.capacity() * sizeof(TextVertex), vertexData.data(), GL_DYNAMIC_DRAW);
    }
}
