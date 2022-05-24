#include "triangle_renderer.hpp"

namespace ldwb
{

TriangleRenderer::TriangleRenderer(Shader shader) :
    frame_pos_(0.0f, 0.0f)
{
    shader_ = shader;
    InitRenderData();
}

TriangleRenderer::~TriangleRenderer()
{
    glDeleteVertexArrays(1, &VAO_);
}

glm::vec2 TriangleRenderer::GetFramePosition()
{
    return frame_pos_;
}

void TriangleRenderer::SetFramePosition(glm::vec2 position)
{
    frame_pos_ = position;
}

void TriangleRenderer::IncreaseFramePosition(glm::vec2 position)
{
    frame_pos_ += position;
}

void TriangleRenderer::DrawTriangle(std::array<glm::vec2, 3> triangle, glm::vec3 color)
{
    DrawTriangle(triangle, std::array<glm::vec3, 3>{color, color, color});
}

void TriangleRenderer::DrawTriangle(std::array<glm::vec2, 3> triangle, std::array<glm::vec3, 3> colors)
{
    glBindVertexArray(VAO_);
    for (glm::vec2& vert : triangle)
    {
        vert += frame_pos_;
    }

    //reset the vertices every time
    float vertices[] = 
    { 
        // pos                        //color
        triangle[0].x, triangle[0].y, colors[0].x, colors[0].y, colors[0].z,
        triangle[1].x, triangle[1].y, colors[1].x, colors[1].y, colors[1].z, 
        triangle[2].x, triangle[2].y, colors[2].x, colors[2].y, colors[2].z
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);
    
    shader_.SetMatrix4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void TriangleRenderer::InitRenderData()
{
    //configure the VAO and VBO
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 5, NULL, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

} //namespace ldwb