#include "Cursor.h"
#include "glad/glad.h"

void Cursor::render()
{
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate and bind VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Vertex data for the cursor line
        GLfloat vertices[] = {
        posInPlane.x, posInPlane.y, 0.0f,
        posInPlane.x+cursorWidth, posInPlane.y, 0.0f,
        posInPlane.x, posInPlane.y+cursorHeight, 0.0f,

        posInPlane.x,posInPlane.y+cursorHeight, 0.0f,
        posInPlane.x+cursorWidth, posInPlane.y, 0.0f,
        posInPlane.x+cursorWidth, posInPlane.y+cursorHeight, 0.0f,
        };

        // Copy vertex data to VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Specify the layout of the vertex data
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
       
        // Unbind VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindVertexArray(vao);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
}