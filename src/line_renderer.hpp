#ifndef LINE_RENDERER_HPP
#define LINE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

namespace ldwb
{

void LinePointsToOpenGLPosition(glm::vec2 pos_1, glm::vec2 pos_2, float size, glm::vec2& line_pos_for_opengl, glm::vec2& line_size, float& rotation);

class LineRenderer
{
public:
    LineRenderer(Shader shader);
    ~LineRenderer();
    glm::vec2 GetFramePosition();
    void SetFramePosition(glm::vec2 position);
    void IncreaseFramePosition(glm::vec2 position);
    //Renders a line
    void DrawLine(glm::vec2 line_pos_for_opengl, glm::vec2 line_size, float theta_degrees, glm::vec3 color);
    void DrawLineFromPos(glm::vec2 pos_1, glm::vec2 pos_2, float size = 10.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    //Members for OpenGL state
    Shader       shader_; 
    unsigned int VAO_;
    glm::vec2 frame_pos_;
    //Initializes and configures the quad's buffer and vertex attributes
    void InitRenderData();
};

} //namespace ldwb

#endif