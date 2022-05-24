#include "line_object.hpp"

namespace ldwb
{

LineObject::LineObject() 
    : pos_1_(0.0f, 0.0f), pos_2_(1.0f, 1.0f), size_(1.0f, 1.0f), color_(1.0f), rotation_(0.0f) { }

LineObject::LineObject(glm::vec2 pos_1, glm::vec2 pos_2, float size, glm::vec3 color) 
    : pos_1_(pos_1), pos_2_(pos_2), color_(color)
{
    LinePointsToOpenGLPosition(pos_1_, pos_2_, size, pos_for_opengl_, size_, rotation_);
}

void LineObject::Draw(LineRenderer &renderer)
{
    renderer.DrawLine(pos_for_opengl_, size_, rotation_, color_);
}

//DotObject contains the data to make a point of color "color" centered at the center_point provided.
//size sets the width/height of the "dot" drawn around that point.
DotObject::DotObject() : LineObject() { }

DotObject::DotObject(glm::vec2 center_point, float size, glm::vec3 color)
    : LineObject(glm::vec2(center_point.x - (size/2.0f), center_point.y), glm::vec2(center_point.x + (size/2.0f), center_point.y), size, color)
{

}

} //namespace ldwb