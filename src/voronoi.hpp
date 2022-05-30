#ifndef VORONOI_HPP
#define VORONOI_HPP

#include <glm/glm.hpp>

//for Voronoi diagram creation
#include <MyGAL/FortuneAlgorithm.h>

#include "line_object.hpp"
#include "triangle_object.hpp"

namespace ldwb
{

//Container object for a Voronoi graph and objects to draw it on screen
class Voronoi
{
public:
    // constructor(s)
    Voronoi();
    Voronoi(glm::vec2 offset, glm::vec2 size);
    void ResetDiagram(int num_points);
    void GenerateDiagram(const std::vector<mygal::Vector2<double>>& points);
    void RelaxDiagram();
    void AddPoint(glm::vec2 point);
    void UpdateLines();
    // draw line
    void DrawSiteColor(TriangleRenderer &tri_renderer);
    void DrawSitePoint(LineRenderer &line_renderer);
    void DrawSiteBorder(LineRenderer &line_renderer);
private:
    std::unique_ptr<mygal::Diagram<double>> diagram_;
    std::vector<mygal::Vector2<double>> points_;
    glm::vec2 offset_;
    glm::vec2 size_;
    std::vector<glm::vec3> colors_;
    std::vector<LineObject> lines_;
    std::vector<DotObject> dots_;
    std::vector<TriangleObject> triangles_;
};

} //namespace ldwb

#endif