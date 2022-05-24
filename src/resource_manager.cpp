#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

//For getting path of executable
//TODO: adjust for non-windows machines
#include <windows.h>

namespace ldwb
{

// Initialize static variables
std::map<std::string, Shader> ResourceHandler::shaders_;

namespace
{
//TODO: add non-windows code
std::string GetExePath() 
{
    char buffer[MAX_PATH];

    GetModuleFileName(NULL, buffer, MAX_PATH);

    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    if (pos == std::string::npos)
    {
        return "";
    }
    else 
    {
        return std::string(buffer).substr(0, pos);
    }
}
} //namespace

Shader ResourceHandler::LoadShader(std::string v_shader_file_path, std::string f_shader_file_path, std::string g_shader_file_path, std::string name)
{
    std::string exe_path = GetExePath();
    v_shader_file_path = exe_path + "/" + v_shader_file_path;
    f_shader_file_path = exe_path + "/" + f_shader_file_path;
    if (g_shader_file_path.length() > 0)
    {
        g_shader_file_path = exe_path + "/" + g_shader_file_path;
        shaders_[name] = LoadShaderFromFile(v_shader_file_path.c_str(), f_shader_file_path.c_str(), g_shader_file_path.c_str());
    }
    else
        shaders_[name] = LoadShaderFromFile(v_shader_file_path.c_str(), f_shader_file_path.c_str(), nullptr);
    return shaders_[name];
}

Shader ResourceHandler::GetShader(std::string name)
{
    return shaders_[name];
}

void ResourceHandler::Clear()
{
    //delete all shaders	
    for (auto iter : shaders_)
    {
        glDeleteProgram(iter.second.ID_);
    }
}

Shader ResourceHandler::LoadShaderFromFile(const char *v_shader_file_path, const char *f_shader_file_path, const char *g_shader_file_path)
{
    // retrieve the shader source code from each file_path
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;
    try
    {
        //open each file
        std::ifstream vertex_shader_file(v_shader_file_path);
        std::ifstream fragment_shader_file(f_shader_file_path);
        //read each file's buffer contents into streams
        std::stringstream v_shader_stream, f_shader_stream;
        v_shader_stream << vertex_shader_file.rdbuf();
        f_shader_stream << fragment_shader_file.rdbuf();
        //close the file handlers
        vertex_shader_file.close();
        fragment_shader_file.close();
        //convert each stream into a string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
        //if a geometry shader path was included, do the above for the geometry shader
        if (g_shader_file_path != nullptr)
        {
            std::ifstream geometry_shader_file(g_shader_file_path);
            std::stringstream gShaderStream;
            gShaderStream << geometry_shader_file.rdbuf();
            geometry_shader_file.close();
            geometry_code = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR: Shader: Failed to read all shader files" << std::endl;
    }
    const char *v_shader_code = vertex_code.c_str();
    const char *f_shader_code = fragment_code.c_str();
    const char *g_shadercode = geometry_code.c_str();
    //create the Shader object from the loaded source code
    Shader shader;
    shader.Compile(v_shader_code, f_shader_code, g_shader_file_path != nullptr ? g_shadercode : nullptr);
    return shader;
}

} //namespace ldwb