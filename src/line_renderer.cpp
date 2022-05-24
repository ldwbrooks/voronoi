#include "line_renderer.hpp"

namespace ldwb
{

LineRenderer::LineRenderer(Shader shader) :
    frame_pos_(0.0f, 0.0f)
{
    shader_ = shader;
    InitRenderData();
}

LineRenderer::~LineRenderer()
{
    glDeleteVertexArrays(1, &VAO_);
}

glm::vec2 LineRenderer::GetFramePosition()
{
    return frame_pos_;
}

void LineRenderer::SetFramePosition(glm::vec2 position)
{
    frame_pos_ = position;
}

void LineRenderer::IncreaseFramePosition(glm::vec2 position)
{
    frame_pos_ += position;
}

void LineRenderer::DrawLine(glm::vec2 line_pos_for_opengl, glm::vec2 line_size, float theta_degrees, glm::vec3 color)
{
    //prepare transformations
    line_pos_for_opengl += frame_pos_;
    shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);

    //transformations happen in the order of scale, then rotation, and finally translation; reversed order for the OpenGL code here
    model = glm::translate(model, glm::vec3(line_pos_for_opengl, 0.0f));

    //translate for the rotation point, then rotate, then translate back
    model = glm::translate(model, glm::vec3(0.5f * line_size.x, 0.5f * line_size.y, 0.0f));
    model = glm::rotate(model, glm::radians(theta_degrees), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * line_size.x, -0.5f * line_size.y, 0.0f));

    model = glm::scale(model, glm::vec3(line_size.x, line_size.y, 1.0f));

    shader_.SetMatrix4("model", model);

    shader_.SetVector3f("spriteColor", color);

    //render the textured quad
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void LinePointsToOpenGLPosition(glm::vec2 pos_1, glm::vec2 pos_2, float size, glm::vec2& line_pos_for_opengl, glm::vec2& line_size, float& rotation)
{
    glm::vec2 halfway_pt((pos_1.x + pos_2.x) / 2.0f, (pos_1.y + pos_2.y) / 2.0f);
    float delta_x = pos_2.x - pos_1.x;
    float delta_y = pos_2.y - pos_1.y;
    float dist_between_points = sqrtf((delta_x * delta_x) + (delta_y * delta_y));
    line_size = glm::vec2(dist_between_points, size); 
    line_pos_for_opengl = glm::vec2(halfway_pt.x - line_size.x / 2.0f, halfway_pt.y - line_size.y / 2.0f);

    float theta_radians = atan2(delta_y, delta_x);
    float theta_degrees = theta_radians / M_PI * 180;
    rotation = theta_degrees;
}

void LineRenderer::DrawLineFromPos(glm::vec2 pos_1, glm::vec2 pos_2, float size, glm::vec3 color)
{
    glm::vec2 line_pos_for_opengl, line_size;
    float rotation;
    LinePointsToOpenGLPosition(pos_1, pos_2, size, line_pos_for_opengl, line_size, rotation);
    DrawLine(line_pos_for_opengl, line_size, rotation, color);
}

void LineRenderer::InitRenderData()
{
    //configure the VAO and VBO
    unsigned int VBO;
    float vertices[] = { 
        //pos
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

} //namespace ldwb