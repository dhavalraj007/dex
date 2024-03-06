#include "shader.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &vertexSourcePath,
               const std::string &fragmentSourcePath) // creates shader program from shader source
{
    std::ifstream vertexFile(vertexSourcePath);
    std::ifstream fragFile(fragmentSourcePath);

    std::stringstream vS, fS;
    vS << vertexFile.rdbuf();
    fS << fragFile.rdbuf();
    std::string vertexSource = vS.str();
    std::string fragmentSource = fS.str();

    m_ProgramId = glCreateProgram();

    int status = GL_FALSE;
    char errorLog[512];
    uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    {
        const GLchar *vertSource = vertexSource.c_str();
        glShaderSource(vertexShaderId, 1, &vertSource, NULL);
        glCompileShader(vertexShaderId);
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            glGetShaderInfoLog(vertexShaderId, sizeof(errorLog), NULL, errorLog);
            std::cout << std::format("Vertex Shader Compilation Error : {}", errorLog);
        }
        else
        {
            glAttachShader(m_ProgramId, vertexShaderId);
        }
    }

    uint32_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    if (status == GL_TRUE)
    {
        const GLchar *fragSource = fragmentSource.c_str();
        glShaderSource(fragmentShaderId, 1, &fragSource, NULL);
        glCompileShader(fragmentShaderId);
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            glGetShaderInfoLog(fragmentShaderId, sizeof(errorLog), NULL, errorLog);
            std::cout << std::format("Fragment Shader Compilation Error: {}", errorLog);
        }
        else
        {
            glAttachShader(m_ProgramId, fragmentShaderId);
        }
    }

    // MCLONE_ASSERT(status == GL_TRUE, "Error Compiling Shader");
    if (status == GL_TRUE)
    {
        glLinkProgram(m_ProgramId);
        glValidateProgram(m_ProgramId);
        glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &status);
        if (status != GL_TRUE)
        {
            glGetProgramInfoLog(m_ProgramId, sizeof(errorLog), NULL, errorLog);
            std::cout << std::format("shader link Error: {}", errorLog);
            glDeleteProgram(m_ProgramId);
            m_ProgramId = -1; // inavlid value
        }
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(m_ProgramId);
}

void Shader::bind()
{
    glUseProgram(m_ProgramId);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::setUniformInt(const std::string &name, int val)
{
    bind();
    glUniform1i(getUniformLocation(name), val);
}

void Shader::setUniformInt2(const std::string &name, int val1, int val2)
{
    bind();
    glUniform2i(getUniformLocation(name), val1, val2);
}

void Shader::setUniformInt2(const std::string &name, const glm::ivec2 &val)
{
    setUniformInt2(name, val.x, val.y);
}

void Shader::setUniformInt3(const std::string &name, int val1, int val2, int val3)
{
    bind();
    glUniform3i(getUniformLocation(name), val1, val2, val3);
}

void Shader::setUniformInt3(const std::string &name, const glm::ivec3 &val)
{
    setUniformInt3(name, val.x, val.y, val.z);
}

void Shader::setUniformInt4(const std::string &name, int val1, int val2, int val3, int val4)
{
    bind();
    glUniform4i(getUniformLocation(name), val1, val2, val3, val4);
}

void Shader::setUniformInt4(const std::string &name, const glm::ivec4 &val)
{
    setUniformInt4(name, val.x, val.y, val.z, val.w);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void Shader::setUniformFloat(const std::string &name, float val)
{
    bind();
    glUniform1f(getUniformLocation(name), val);
}

void Shader::setUniformFloat2(const std::string &name, float val1, float val2)
{
    bind();
    glUniform2f(getUniformLocation(name), val1, val2);
}

void Shader::setUniformFloat2(const std::string &name, const glm::vec2 &val)
{
    setUniformFloat2(name, val.x, val.y);
}

void Shader::setUniformFloat3(const std::string &name, float val1, float val2, float val3)
{
    bind();
    glUniform3f(getUniformLocation(name), val1, val2, val3);
}

void Shader::setUniformFloat3(const std::string &name, const glm::vec3 &val)
{
    setUniformFloat3(name, val.x, val.y, val.z);
}

void Shader::setUniformFloat4(const std::string &name, float val1, float val2, float val3, float val4)
{
    bind();
    glUniform4f(getUniformLocation(name), val1, val2, val3, val4);
}

void Shader::setUniformFloat4(const std::string &name, const glm::vec4 &val)
{
    setUniformFloat4(name, val.x, val.y, val.z, val.w);
}

void Shader::setUniformMat3(const std::string &name, const glm::mat3 &mat)
{
    bind();
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniformMat4(const std::string &name, const glm::mat4 &mat)
{
    bind();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

int Shader::getUniformLocation(const std::string &name)
{
    if (m_uniformLocations.find(name) == m_uniformLocations.end())
    {
        m_uniformLocations[name] = glGetUniformLocation(m_ProgramId, name.c_str());
    }
    return m_uniformLocations[name];
}