#include "shader.hpp"

#include <iostream>

namespace ldwb
{

Shader& Shader::Use()
{
    glUseProgram(this->ID_);
    return *this;
}

void Shader::Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
    unsigned int vertex_id, fragment_id, geometry_id;
    //vertex Shader
    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);
    CheckCompileErrors(vertex_id, "VERTEX");
    //fragment Shader
    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_source, NULL);
    glCompileShader(fragment_id);
    CheckCompileErrors(fragment_id, "FRAGMENT");

    //compile geometry shader only if provided
    if (geometry_source != nullptr)
    {
        geometry_id = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_id, 1, &geometry_source, NULL);
        glCompileShader(geometry_id);
        CheckCompileErrors(geometry_id, "GEOMETRY");
    }

    //create the OpenGL shader program
    this->ID_ = glCreateProgram();
    glAttachShader(this->ID_, vertex_id);
    glAttachShader(this->ID_, fragment_id);
    if (geometry_source != nullptr)
    {
        glAttachShader(this->ID_, geometry_id);
    }
    glLinkProgram(this->ID_);
    CheckCompileErrors(this->ID_, "PROGRAM");
    //delete the shader components as they're now compiled into the program and no longer necessary
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
    if (geometry_source != nullptr)
    {
        glDeleteShader(geometry_id);
    }
}

void Shader::SetFloat(const char *name, float value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->ID_, name), value);
}

void Shader::SetInteger(const char *name, int value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->ID_, name), value);
}

void Shader::SetVector2f(const char *name, float x, float y, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID_, name), x, y);
}

void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID_, name), value.x, value.y);
}

void Shader::SetVector3f(const char *name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID_, name), x, y, z);
}

void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID_, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID_, name), x, y, z, w);
}

void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID_, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID_, name), 1, false, glm::value_ptr(matrix));
}


void Shader::CheckCompileErrors(unsigned int object, std::string type)
{
    int success;
    char error_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, error_log);
            std::cout << "ERROR: Shader: Compile-time error: Type: " << type << "\n"
                << error_log << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, error_log);
            std::cout << "ERROR: Shader: Link-time error: Type: " << type << "\n"
                << error_log << std::endl;
        }
    }
}

} //namespace ldwb