#include "rasterizer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Rasterizer::Rasterizer(const std::vector<Polygon>& polygons)
    : m_polygons(polygons)
{
    m_zBuffer.resize(512 * 512, std::numeric_limits<double>::max());
}
//update camera
Camera& Rasterizer::getCamera() {
    return m_camera;
}

QImage Rasterizer::RenderScene()
{
    QImage result(512, 512, QImage::Format_RGB32);
    result.fill(qRgb(0.f, 0.f, 0.f));
    for(auto& poly: m_polygons){
        for(auto & tri: poly.m_tris){
            Vertex& v1 = poly.m_verts[tri.m_indices[0]];
            Vertex& v2 = poly.m_verts[tri.m_indices[1]];
            Vertex& v3 = poly.m_verts[tri.m_indices[2]];

            glm::vec2 v1_screen = glm::vec2(v1.m_pos);
            glm::vec2 v2_screen = glm::vec2(v2.m_pos);
            glm::vec2 v3_screen = glm::vec2(v3.m_pos);

            std::array<float, 4> box = boudingBox(v1_screen, v2_screen, v3_screen);
            for(int y = box[2]; y <= box[3]; y++){
                float xLeft = 512;
                float xRight = 0;

                Segment s1 ({v1.m_pos.x, v1.m_pos.y}, {v2.m_pos.x, v2.m_pos.y});
                Segment s2 ({v2.m_pos.x, v2.m_pos.y}, {v3.m_pos.x, v3.m_pos.y});
                Segment s3 ({v3.m_pos.x, v3.m_pos.y}, {v1.m_pos.x, v1.m_pos.y});

                std::vector<Segment> segments = {s1,s2, s3};

                for(auto& segment: segments){

                    float xIntersection;

                    if(segment.getIntersection(y, &xIntersection)){
                        xLeft = std::min(xLeft, xIntersection);
                        xRight = std::max(xRight, xIntersection);
                    }
                }

                //test two object, if object overwahlm screen, it will crash
                if (y < 0 || y >= 512) continue;
                for(int x = (int)xLeft; x<= (int)xRight; x++){
                    glm::vec2 point(x, y);
                    if(IsInsideTriangle(v1_screen, v2_screen,v3_screen, point)){
                        glm::vec3 baryCoord = BarycentricInterpolation(v1_screen, v2_screen,v3_screen, point);
                        double depth = baryCoord.x * v1.m_pos.z +
                                       baryCoord.y * v2.m_pos.z +
                                       baryCoord.z * v3.m_pos.z;

                        // Z-buffer check
                        if (x < 0 || x >= 512) continue;
                        int index = x + 512 * y;
                        if (depth < m_zBuffer[index])
                        {
                            m_zBuffer[index] = depth;
                            // Interpolate color using barycentric coordinates
                            glm::vec3 color = baryCoord.x * v1.m_color +
                                              baryCoord.y * v2.m_color +
                                              baryCoord.z * v3.m_color;
                            result.setPixel(x, y, qRgb(color.r, color.g, color.b));
                        }
                    }
                }
            }

        }

    }
    return result;
}




//thank you Yuhan
std::array<float, 4> Rasterizer::boudingBox(glm::vec2& v1, glm::vec2& v2, glm::vec2& v3){
    std::array<float,4> box;
    box[0] = std::max(0.0f, std::min(v1.x, std::min(v2.x, v3.x))); //min x
    box[1] = std::min(511.f, std::max(v1.x, std::max(v2.x, v3.x))); //max x
    box[2] = std::max(0.f, std::min(v1.y, std::min(v2.y, v3.y)));// min y
    box[3] = std::max(511.f, std::max(v1.y, std::max(v2.y, v3.y)));// min y
    return box;
}


glm::vec3 Rasterizer::BarycentricInterpolation(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3,glm::vec2& point)
{
    float totalArea = ComputeArea(p1, p2, p3);
    float area1 = ComputeArea(point, p2, p3);
    float area2 = ComputeArea(point, p1, p3);
    float area3 = ComputeArea(point, p1, p2);
    return glm::vec3(area1 / totalArea, area2 / totalArea, area3 / totalArea);
}


// Check if the point is inside the triangle using barycentric coordinates
bool Rasterizer::IsInsideTriangle(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3,glm::vec2& point)
{
    glm::vec3 baryCoord = BarycentricInterpolation(p1, p2, p3,point);
    return baryCoord.x >= 0 && baryCoord.y >= 0 && baryCoord.z >= 0;
}


// Compute the area of a triangle
float Rasterizer::ComputeArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    return 0.5f * abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
}


void Rasterizer::ClearScene()
{
    m_polygons.clear();
}
Rasterizer::~Rasterizer() {
    m_polygons.clear();
    m_zBuffer.clear();
}
