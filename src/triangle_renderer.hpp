#ifndef TRIANGLE_RENDERER_HPP
#define TRIANGLE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include <array>

namespace ldwb
{

class TriangleRenderer
{
public:
    TriangleRenderer(Shader shader);
    ~TriangleRenderer();
    glm::vec2 GetFramePosition();
    void SetFramePosition(glm::vec2 position);
    void IncreaseFramePosition(glm::vec2 position);
    //Renders a triangle of the given color(s)
    void DrawTriangle(std::array<glm::vec2, 3> triangle, glm::vec3 color);
    void DrawTriangle(std::array<glm::vec2, 3> triangle, std::array<glm::vec3, 3> colors);
private:
    //Members for OpenGL state
    Shader shader_; 
    unsigned int VAO_;
    unsigned int VBO_;
    glm::vec2 frame_pos_;
    //Initializes and configures the quad's buffer and vertex attributes
    void InitRenderData();
};

} //namespace ldwb

#endif