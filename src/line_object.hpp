#ifndef LINE_OBJECT_HPP
#define LINE_OBJECT_HPP

#include <glm/glm.hpp>

#include "line_renderer.hpp"

namespace ldwb
{

//Container object for lines to be drawn.
class LineObject
{
public:
    glm::vec2   pos_1_, pos_2_, pos_for_opengl_, size_;
    glm::vec3   color_;
    float       rotation_;
    //constructors
    LineObject();
    LineObject(glm::vec2 pos_1, glm::vec2 pos_2, float size, glm::vec3 color);
    //function for drawing the line on screen
    void Draw(LineRenderer &renderer);
};

//DotObject contains the data to make a point of color "color" centered at the center_point provided.
//size sets the width/height of the "dot" drawn around that point.
class DotObject : public LineObject
{
public:
    DotObject();
    DotObject(glm::vec2 center_point, float size, glm::vec3 color);
};

} //namespace ldwb

#endif