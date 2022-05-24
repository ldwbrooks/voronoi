#include "triangle_object.hpp"

namespace ldwb
{

TriangleObject::TriangleObject()
    : TriangleObject({glm::vec2(1.0f, 1.0f), glm::vec2(1.0f,1.0f), glm::vec2(1.0f,1.0f)}, glm::vec3(1.0f))
{ }

TriangleObject::TriangleObject(glm::vec2 pos_1, glm::vec2 pos_2, glm::vec2 pos_3, glm::vec3 color)
    : TriangleObject({pos_1, pos_2, pos_3}, color)
{ }

TriangleObject::TriangleObject(std::array<glm::vec2, 3> vertices, glm::vec3 color)
    : vertices_(vertices), color_(color)
{ }

void TriangleObject::Draw(TriangleRenderer &renderer)
{
    renderer.DrawTriangle(vertices_, color_);
}

} //namespace ldwb