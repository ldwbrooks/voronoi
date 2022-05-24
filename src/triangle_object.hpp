#ifndef TRIANGLE_OBJECT_HPP
#define TRIANGLE_OBJECT_HPP

#include <array>
#include <glm/glm.hpp>

#include "triangle_renderer.hpp"

namespace ldwb
{

//Container object for Triangles to be drawn.
class TriangleObject
{
public:
    std::array<glm::vec2, 3> vertices_;
    glm::vec3 color_;
    //constructors
    TriangleObject();
    TriangleObject(glm::vec2 pos_1, glm::vec2 pos_2, glm::vec2 pos_3, glm::vec3 color);
    TriangleObject(std::array<glm::vec2, 3> vertices, glm::vec3 color);
    //draw the Triangle on the screen
    void Draw(TriangleRenderer &renderer);
};

} //namespace ldwb

#endif