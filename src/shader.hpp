#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ldwb
{

/* 
General purpose shader object. Compiles shaders from GLSL data,
shows any compile/link-time error messages,
and saves them for later if successful.
*/
class Shader
{
public:
    unsigned int ID_; 
    Shader() { }
    //sets this shader as the active shader
    Shader& Use();
    //compiles the shader from given source code
    void Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr);
    //utility functions
    void SetFloat    (const char *name, float value, bool use_shader = false);
    void SetInteger  (const char *name, int value, bool use_shader = false);
    void SetVector2f (const char *name, float x, float y, bool use_shader = false);
    void SetVector2f (const char *name, const glm::vec2 &value, bool use_shader = false);
    void SetVector3f (const char *name, float x, float y, float z, bool use_shader = false);
    void SetVector3f (const char *name, const glm::vec3 &value, bool use_shader = false);
    void SetVector4f (const char *name, float x, float y, float z, float w, bool use_shader = false);
    void SetVector4f (const char *name, const glm::vec4 &value, bool use_shader = false);
    void SetMatrix4  (const char *name, const glm::mat4 &matrix, bool use_shader = false);
private:
    //checks if compilation and/or linking failed and prints the OpenGL error logs if there was an error
    void    CheckCompileErrors(unsigned int object, std::string type); 
};

} //namespace ldwb

#endif