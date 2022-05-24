#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <map>

#include <glad/glad.h>

#include "shader.hpp"

namespace ldwb
{
/*
A singleton-style static ResourceHandler class that loads Shaders. 
The loaded shaders are saved for future reference by string handles.
*/
class ResourceHandler
{
public:
    //static maps for resource storage
    static std::map<std::string, Shader> shaders_;
    //loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader.
    static Shader LoadShader(std::string v_shader_file_path, std::string f_shader_file_path, std::string g_shader_file_path, std::string name);
    // retrieves a stored sader
    static Shader GetShader(std::string name);
    // properly de-allocates all loaded resources
    static void Clear();
private:
    // The constructor is made private so that it is impossible to make any actual resource manager objects, as its functions and members are statically available.
    ResourceHandler() { }
    // Loads and generates a shader from file
    static Shader LoadShaderFromFile(const char *v_shader_file_path, const char *f_shader_file_path, const char *g_shader_file_path = nullptr);
};

} //namespace ldwb

#endif

