#include "voronoi.hpp"
#include <chrono>
#include <random>

#include <mapbox/earcut.hpp>

namespace ldwb
{

namespace 
{
std::vector<mygal::Vector2<double>> GeneratePoints(int num_points)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto generator = std::default_random_engine(seed);
    auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    auto points = std::vector<mygal::Vector2<double>>(num_points);
    for (auto i = 0; i < num_points; ++i)
        points[i] = mygal::Vector2<double>(distribution(generator), distribution(generator));

    return points;
}

glm::vec3 GenerateColor()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto generator = std::default_random_engine(seed);
    auto distribution = std::uniform_real_distribution<double>(0.0, 1.0);

    glm::vec3 color(distribution(generator), distribution(generator), distribution(generator));

    return color;
}
}

Voronoi::Voronoi() 
    : Voronoi(glm::vec2(0.0f), glm::vec2(1000.0f))
{ 
}

Voronoi::Voronoi(glm::vec2 offset, glm::vec2 size)
    : offset_(offset), size_(size)
{
    ResetDiagram(10);
}

void Voronoi::ResetDiagram(int num_points)
{
    auto points = GeneratePoints(num_points);
    colors_.clear();
    for (int i = 0; i < num_points; i++)
        colors_.push_back(GenerateColor());
    GenerateDiagram(points);
}

void Voronoi::GenerateDiagram(const std::vector<mygal::Vector2<double>>& points)
{
    points_ = points; //Save points so that the user can add a new point to the existing points later
    auto algorithm = mygal::FortuneAlgorithm<double>(points); //Initialize an instance of Fortune's algorithm
    algorithm.construct(); //Construct the diagram
    algorithm.bound(mygal::Box<double>{-0.05, -0.05, 1.05, 1.05}); //Bound the diagram
    diagram_ = std::make_unique<mygal::Diagram<double>>(algorithm.getDiagram()); //Get the constructed diagram
    diagram_->intersect(mygal::Box<double>{0.00, 0.00, 1.00, 1.00}); //Compute the intersection between the diagram and a box
    UpdateLines();
}

void Voronoi::RelaxDiagram()
{
    auto relaxed_points = diagram_->computeLloydRelaxation();
    GenerateDiagram(relaxed_points);
}

void Voronoi::AddPoint(glm::vec2 point)
{
    mygal::Vector2<double> pt(point.x, point.y);
    //check for equality between the new point's y and existing points' y
    //if equal, offset it just a little so that the algorithms can handle it
    for (auto& existing_point : points_)
    {
        if (pt.y == existing_point.y)
        {
            pt.y += 0.001f;
        }
    }
    points_.push_back(pt);
    colors_.push_back(GenerateColor());
    GenerateDiagram(points_);
}

namespace
{
std::vector<std::array<std::array<float, 2>, 3>> TriangulatePolygon(std::vector<std::vector<std::array<float, 2>>> polygon)
{
    // Run tessellation
    // Returns array of indices that refer to the vertices of the input polygon.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    std::vector<std::array<std::array<float, 2>, 3>> result;
    for (int i = 2; i < indices.size(); i = i + 3)
    {
        int outer_vec = 0; // i / polygon[0].size();
        std::array<std::array<float, 2>, 3> tri{polygon[outer_vec][indices[i - 2]], polygon[outer_vec][indices[i - 1]], polygon[outer_vec][indices[i - 0]]};
        result.push_back(tri);
    }
    return result;
}
}

void Voronoi::UpdateLines()
{
    dots_.clear();
    lines_.clear();
    triangles_.clear();
    const auto& sites = diagram_->getSites();

    for (int site_idx = 0; site_idx < sites.size(); site_idx++)
    {
        const auto& site = sites[site_idx];
        auto center = site.point;
        center.x *= size_.x;
        center.y *= size_.y;
        center.x += offset_.x;
        center.y += offset_.y;

        //save center information for rendering
        DotObject dot(glm::vec2(center.x, center.y), 20.0f, glm::vec3(1.0f));
        dots_.push_back(dot);

        auto face = site.face;
        auto halfEdge = face->outerComponent;
        if (halfEdge == nullptr)
            continue;
        while (halfEdge->prev != nullptr)
        {
            halfEdge = halfEdge->prev;
            if (halfEdge == face->outerComponent)
                break;
        }
        auto start = halfEdge;
        std::vector<std::array<float, 2>> vertices;
        while (halfEdge != nullptr)
        {
            if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
            {
                mygal::Vector2<double> offset(offset_.x, offset_.y);
                auto origin = (halfEdge->origin->point - center) * double(1.0f) + center;
                glm::vec2 or_pt(origin.x * size_.x, origin.y * size_.y);
                or_pt += offset_;

                auto destination = (halfEdge->destination->point - center) * double(1.0f) + center;
                glm::vec2 dest_pt(destination.x * size_.x, destination.y * size_.y);
                dest_pt += offset_;

                //get vertices for face rendering
                std::array<float, 2> or_arr = {or_pt.x, or_pt.y};
                std::array<float, 2> dest_arr = {or_pt.x, or_pt.y};
                vertices.push_back(or_arr);
                vertices.push_back(dest_arr);

                //get lines and dots for border rendering
                LineObject line(or_pt, dest_pt, 10.0f, glm::vec3(0.2f));
                lines_.push_back(line);
                dots_.push_back(DotObject(or_pt, 10.0f, glm::vec3(0.5f)));
                dots_.push_back(DotObject(dest_pt, 10.0f, glm::vec3(0.5f)));
            }
            halfEdge = halfEdge->next;
            if (halfEdge == start)
            {
                //set up necessary information for rendering filled in faces
                std::vector<std::array<std::array<float, 2>, 3>> result = TriangulatePolygon({vertices});
                for (auto& triangle : result)
                {
                    TriangleObject tri_obj({glm::vec2(triangle[0][0], triangle[0][1]), glm::vec2(triangle[1][0], triangle[1][1]), glm::vec2(triangle[2][0], triangle[2][1])}, 
                        colors_[site_idx]);
                    triangles_.push_back(tri_obj);
                }
                break;
            }
        }
    }
}

void Voronoi::DrawSiteColor(TriangleRenderer &tri_renderer)
{
    for (auto& tri : triangles_)
        tri.Draw(tri_renderer);
}

void Voronoi::DrawSitePoint(LineRenderer &line_renderer)
{
    for (auto& dot : dots_)
        dot.Draw(line_renderer);
}

void Voronoi::DrawSiteBorder(LineRenderer &line_renderer)
{
    for (auto& line : lines_)
        line.Draw(line_renderer);
}

} //namespace ldwb